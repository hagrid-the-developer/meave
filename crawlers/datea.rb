require 'curb'
require 'nokogiri'
require 'cgi'
require 'yaml'


URL_PREFIX = 'http://www.datea.dk'
URL = URL_PREFIX + '/Default.aspx?ID=1374&pnrfr=9999&pnrto=0&krit=0&kritTo=0'
UA = 'Mozilla/4.0 (compatible; MSIE 4.5; Mac_PowerPC)'

def parse_price(price_str)
    price_str.match(%r{\d+([.]\d+)*(,\d+)?}) { |m| return m[0].gsub(%r{[.]}, '').gsub(%r{,}, '.').to_f }
    return nil
end

def parse_postnum(addr)
    addr.match(%r{\d{4}}) { |m| return m[0] }
    return nil
end

def parse_final_page(url)
    puts "#{url}"
    begin
        c = Curl::Easy.new(url)
        c.useragent = UA
        # FIXME: Check HTTP GET result
        c.http_get
        doc = Nokogiri::HTML(c.body_str)
        c.close
    rescue Curl::Err::ConnectionFailedError
        return []
    end

    address = doc.xpath('//td/h1').text.strip
    postnum = parse_postnum address
    price = parse_price doc.at_xpath('//td[contains(text(), "Leje pr.")]/following-sibling::td/following-sibling::td').text
    imgs = doc.xpath('//div[@id = "slider"]//img').map { |n| n['src'].strip }

    return {
        'url' => url,
        'title' => address,
        'address' => address,
        'price' => price,
        'zip' => postnum,
        'imgs' => imgs.join(','),
    }
end

c = Curl::Easy.new(URL)
c.useragent = UA
# FIXME: Check HTTP GET result
c.http_get
doc = Nokogiri::HTML(c.body_str)
c.close

#rentals = doc.xpath('//a[contains(@id, "ctl00_content_body_c_uc_search_rep_houses_")]').map do |node_a|
rentals = doc.xpath('//a/img/..').map do |node_a|
    href = URL_PREFIX + node_a['href'].strip
    parse_final_page(href)
end

###

rentals.flatten!


File.open('test.yaml', 'w') do |out|
  out.write(YAML::dump(rentals))
end
