require 'curb'
require 'nokogiri'
require 'cgi'
require 'yaml'


URL_PREFIX = 'http://www.bsmidtvest.dk'
URL = URL_PREFIX + '/Search.aspx?KunLedige=true'
URL_POST = URL_PREFIX + '/SearchResult/Afdeling.aspx'
UA = 'Mozilla/4.0 (compatible; MSIE 4.5; Mac_PowerPC)'

def parse_price(price_str)
    price_str.match(%r{\d+([.]\d+)*(,\d+)?}) { |m| return m[0].gsub(%r{[.]}, '').gsub(%r{,}, '.').to_f }
    return nil
end

def parse_postnum(addr)
    addr.match(%r{\d{4}}) { |m| return m[0] }
    return nil
end

def parse_postkey(id)
    id.match(%r{_(\d+_\d+)$}) { |m| return m[1] }
    return nil
end

def load_images(postkey)
    c = Curl::Easy.new(URL_POST)
    c.useragent = UA
    # FIXME: Check HTTP GET result
    c.http_post("key=#{postkey}")
    doc = Nokogiri::HTML(c.body_str)
    c.close

    imgs = doc.xpath(".//a[@rel=\"floatbox group:pix#{postkey}\"]").map { |n| URL_PREFIX + '/' + n['href'].strip }.uniq
end

def parse_item_node(node)
    address = node.at_css('div.HeaderText').text.strip
    postnum = parse_postnum address
    prices = node.xpath('.//div[@class="ResultRow"]/div[contains(@class, "Result_Husleje")]').map { |n| parse_price n.text }
    id = node.at_xpath('.//div[contains(@class, "BoligTab")]')['id'].strip
    postkey = parse_postkey id
    imgs = load_images(postkey)

    return prices.map do |price|
        {
            'url' => URL + '#' + id,
            'title' => address,
            'address' => address,
            'price' => price,
            'zip' => postnum,
            'imgs' => imgs.join(','),
        }
    end
end

c = Curl::Easy.new(URL)
c.useragent = UA
# FIXME: Check HTTP GET result
c.http_get
doc = Nokogiri::HTML(c.body_str)
c.close

rentals = doc.css('div.SearchResult').map do |n|
    parse_item_node(n)
end

###

rentals.flatten!


File.open('test.yaml', 'w') do |out|
  out.write(YAML::dump(rentals))
end
