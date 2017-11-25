require 'curb'
require 'nokogiri'
require 'cgi'
require 'yaml'


URL_PREFIX = 'http://www.esviborg.dk'
URL = URL_PREFIX + '/'
UA = 'Mozilla/4.0 (compatible; MSIE 4.5; Mac_PowerPC)'

c = Curl::Easy.new(URL)
c.useragent = UA
# FIXME: Check HTTP GET result
c.http_get

doc = Nokogiri::HTML(c.body_str)
c.close

def parse_price(price_str)
    price_str.match(%r{\d+([.]\d+)*(,\d+)?}) { |m| return m[0].gsub(%r{[.]}, '').gsub(%r{,}, '.').to_f }
    return nil
end

def parse_postnum(addr)
    addr.match(%r{\d{4}}) { |m| return m[0] }
    return nil
end

def parse_final_page(url, postnum)
    c = Curl::Easy.new(url)
    c.useragent = UA
    # FIXME: Check HTTP GET result
    c.http_get
    doc = Nokogiri::HTML(c.body_str)
    c.close

    title = doc.at_css('title').text.strip
    address = doc.at_css('h2').text.strip
    price = parse_price doc.at_xpath('//td[contains(text(), "Leje pr.md")]/following-sibling::td').text.strip
    imgs = (doc.css('#bigpicture') + doc.xpath('//img[contains(@onmouseout, "bigpicture")]')).map do |node_img|
        src = node_img['src']
        src.sub %r{/scaled/[^/]*/}, '/scaled/full/'
    end
    return {
        'title' => title,
        'address' => address,
        'price' => price,
        'zip' => postnum,
        'imgs' => imgs.join(','),
    }
end

def parse_middle_page(url, postnum)
    c = Curl::Easy.new(url)
    c.useragent = UA
    # FIXME: Check HTTP GET result
    c.http_get
    doc = Nokogiri::HTML(c.body_str)
    c.close

    return doc.xpath('//table/tr/td/a').map do |node_a|
        href = URL_PREFIX + node_a['href']
        parse_final_page(href, postnum)
    end
end

rentals = doc.xpath('//ul[@id="jsddm"]/li[1]/ul/li/a').map do |node_a|
    href = URL_PREFIX + node_a['href']
    postnum = parse_postnum node_a.text
    parse_middle_page(href, postnum)
end

###

rentals.flatten!
#result.each do |item|
#    puts ">> #{item}"
#end

File.open('test.yaml', 'w') do |out|
  out.write(YAML::dump(rentals))
end
