require 'curb'
require 'nokogiri'
require 'cgi'
require 'yaml'


URL_PREFIX = 'http://www.lejerbo.dk'
URL = URL_PREFIX + '/da/boligsoegende/find-bolig/soegeresultater?Kommuner=1084%7C1082%7C1081%7C1085%7C1083%7C201%7C400%7C153%7C320%7C210%7C147%7C813%7C250%7C190%7C157%7C159%7C161%7C376%7C260%7C217%7C657%7C219%7C316%7C661%7C615%7C167%7C169%7C756%7C849%7C326%7C621%7C101%7C259%7C360%7C173%7C846%7C707%7C450%7C370%7C727%7C730%7C840%7C760%7C230%7C175%7C746%7C330%7C336%7C671%7C787%7C630%7C791%7C390%7C580%7C851%7C751&boligArt=1%7C2%7C3%7C7%7C&BoligFilter=LedigeBoliger&Fritekst='
UA = 'Mozilla/4.0 (compatible; MSIE 4.5; Mac_PowerPC)'

def parse_price(price_str)
    price_str.match(%r{\d+([.]\d+)*(,\d+)?}) { |m| return m[0].gsub(%r{[.]}, '').gsub(%r{,}, '.').to_f }
    return nil
end

def parse_postnum(addr)
    addr.match(%r{\d{4}}) { |m| return m[0] }
    return nil
end

def find_images(url)
    begin
        c = Curl::Easy.new(url)
        c.useragent = UA
        # FIXME: Check HTTP GET result
        c.http_get
        doc = Nokogiri::HTML(c.body_str)
        c.close
        return doc.css('img#bodyregion_0_content_0_mainPic').map {|n| n['src'].strip }
    rescue Curl::Err::ConnectionFailedError
    end
    return []
end

def parse_node(node)
    address = node.xpath('.//a[@class="blackLink"]/following-sibling::text()').map {|n| n.text.strip}.flatten.join(' ')
    postnum = parse_postnum address
    imgs = nil

    return node.xpath('.//a[@class="rightRedArrowLink"]').map do |n|
        url = URL_PREFIX + n['href'].strip.sub(%r{^/boliger/find_bolig/ledig_bolig.aspx[?]}, '/da/boligsoegende/find-bolig/ledig_bolig?')
        price = parse_price n.text.match(%r{-\s*([\d.,]+)\s+kr[.]})[1]
        imgs = find_images(url) unless imgs

        {
            'url' => url,
            'title' => address,
            'address' => address,
            'price' => price,
            'zip' => postnum,
            'imgs' => imgs.join(','),
        }
    end
end

def post_request(doc)
    post_str = {
        '__LASTFOCUS' => '',
        '__EVENTTARGET' => 'bodyregion_0%24content_0%24BtnShowAllTop',
        '__EVENTARGUMENT' => '',
        '__VIEWSTATE' => CGI::escape(doc.at_css('input#__VIEWSTATE')['value'].strip),
        '__EVENTVALIDATION' => CGI::escape(doc.at_css('input#__EVENTVALIDATION')['value'].strip),
        'headerregion_0%24ctl03%24gssQuery' => 'S%C3%B8g+p%C3%A5+Lejerbo',
        'bodyregion_0%24content_0%24DdlSortByTop' => 'Name',
        'bodyregion_0%24content_0%24DdlSortByBottom' => 'Name',
    }.to_a.map { |kv| kv.join '='  }.join '&'
    c = Curl::Easy.new(URL)
    c.useragent = UA
    c.http_post(post_str)
#    puts "#{c.body_str}"
    post_doc = Nokogiri::HTML(c.body_str)
    c.close
    return post_doc
end

c = Curl::Easy.new(URL)
c.useragent = UA
# FIXME: Check HTTP GET result
c.http_get
doc = Nokogiri::HTML(c.body_str)
c.close

doc = post_request(doc)
rentals = doc.xpath('//div[@class="searchResultList"]').map do |n|
    parse_node(n)
end

###

rentals.flatten!

File.open('test.yaml', 'w') do |out|
  out.write(YAML::dump(rentals))
end
