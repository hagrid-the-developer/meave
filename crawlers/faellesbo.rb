require 'curb'
require 'nokogiri'
require 'cgi'
require 'yaml'


URL_PREFIX = 'http://www.faellesbo.dk'
URL = URL_PREFIX + '/S%C3%B8g-bolig/Ledige-boliger/Flere-ledige-boliger.aspx'
UA = 'Mozilla/4.0 (compatible; MSIE 4.5; Mac_PowerPC)'

def parse_price(price_str)
    price_str.match(%r{\d+([.]\d+)*(,\d+)?}) { |m| return m[0].gsub(%r{[.]}, '').gsub(%r{,}, '.').to_f }
    return nil
end

def parse_postnum(addr)
    return nil unless addr
    addr.match(%r{\d{4}}) { |m| return m[0] }
    return nil
end

def parse_final_page(url, list_address)
    puts "href: #{url}"
    begin
        c = Curl::Easy.new(url)
        c.useragent = UA
        # FIXME: Check HTTP GET result
        c.http_get
        doc = Nokogiri::HTML(c.body_str)
        c.close
    rescue Curl::Err::ConnectionFailedError # IPs with too much requests are probably temporarily disabled
        return []
    end

#    address1 = doc.at_xpath('//div[@class="content-title"]/div[@class="holder"]/h2').text.strip
    address_postnum = doc.at_xpath('//div[@class="content-title"]/div[@class="holder"]/h2/following-sibling::strong').text.strip
    postnum = parse_postnum address_postnum
    price = parse_price doc.at_xpath('//dt[contains(text(), "Netto husleje:")]/following-sibling::dd').text.strip
    imgs = (doc.xpath('//div[contains(@class, "gallery-box")]//img')).map do |node_img|
        URL_PREFIX + node_img['src'].strip.sub(%r{^([^/])}) { '/' + $1 }
    end
    title = list_address + '; ' + postnum # FIXME: What title for this site? Element title seems to be useless
    return {
        'url' => url,
        'title' => title,
        'address' => list_address,
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

rentals = doc.xpath('//table[@id="apartmentsTable"]/tbody/tr[contains(@class, "searchresult")]').map do |node|
    list_address = node.xpath('./td[@class="searchresult_2_data"]').map { |n| n.text.strip }.join ', '
    href = URL_PREFIX + node.at_xpath('./td[3]/a')['href'].sub(%r{/([^/?]+)}) { '/' + CGI::escape($1) } # Path does contain non-ascii character and CURL does not handle it correctly
    parse_final_page(href, list_address)
end

###

rentals.flatten!

File.open('test.yaml', 'w') do |out|
  out.write(YAML::dump(rentals))
end
