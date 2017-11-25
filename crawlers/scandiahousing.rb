require 'curb'
require 'nokogiri'
require 'cgi'
require 'yaml'


URL_PREFIX = 'http://www.scandiahousing.com'
URL = URL_PREFIX + '/da/hl/boligudlejning/denmark.aspx'
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

    title = doc.at_css('title').text.strip
    address = doc.at_css('span#ctl00_content_body_c_lbl_area').text.strip
    postnum = nil
    price = parse_price doc.at_css('span#ctl00_content_body_c_lbl_rent').text
    imgs = doc.css('div#galleria > a').map { |node_a| node_a['href'].strip }

    return {
        'url' => url,
        'title' => title,
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
rentals = doc.css('div.area > a').map do |node_a|
    href = node_a['href']
    parse_final_page(href)
end

###

rentals.flatten!


File.open('test.yaml', 'w') do |out|
  out.write(YAML::dump(rentals))
end
