require 'curb'
require 'nokogiri'
require 'cgi'
require 'yaml'


URL_PREFIX = 'http://www.rolf-barfoed.dk'
URL = URL_PREFIX + '/boliglejemal'
UA = 'Mozilla/4.0 (compatible; MSIE 4.5; Mac_PowerPC)'
SLEEP = 2.0
PROVIDER_ID = 8

def parse_price(price_str)
  price_str.match(%r{\d+([.]\d+)*(,\d+)?}) { |m| return m[0].gsub(%r{[.]}, '').gsub(%r{,}, '.').to_f }
  return nil
end

def parse_postnum(addr)
  addr.match(%r{\d{4}}) { |m| return m[0] }
  return nil
end

def parse_property_type(desc)
  return "Apartment" if desc.to_s.downcase.include?("lejlighed")
  return "House"
end

def parse_bedrooms(url)
  url.match(/-([0-9]+)-/)[1]
end

def parse_external_property_id(url)
  url.match(/\/([0-9]+)-/)[1]
end

def parse_description(desc)
  return desc if desc.match(/document.write(.*)\)\)/) == nil
  email = desc.match(/document.write(.*)\)\)/).to_s.scan(/([\"'])(.*?)\1/)
  email_str = email[0][1].to_s+"@"+email[1][1].to_s
  desc.sub(/document.write(.*)\)\)/, email_str)
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

  title = doc.at_css('title').text.strip.sub(/[ -]*NYHED[^a-z]*$/, "")
  address = doc.at_xpath('//p/strong[contains(text(), "Adresse:")]').text.sub(%r{^Adresse:}, '').strip
  streetname = address.match(/[^0-9]+/)[0].strip
  postnum = parse_postnum address
  price = parse_price doc.at_css('div#reditem_infobox_div_12 > div.reditem_text > div.reditem_text_value > p').text
  imgs = doc.css('td.reditem_each_thumbimage_td > img').map { |img_a| URL_PREFIX + img_a['src'] }
  description = parse_description(doc.xpath('//div[@class="seeitem_right_col"]').text.to_s)
  area = doc.xpath('//div[@id = "reditem_infobox_div_9"]//div[@class = "reditem_text_value"]').text
  property_type = parse_property_type description
  ext = Hash.new
  bedrooms = parse_bedrooms url
  ext['external_property_id'] = parse_external_property_id(url).to_i
  ext['provider_id'] = PROVIDER_ID
  ext['property_url'] = url

  return {
      'heading' => title,
      'street_name' => streetname,
      'bedrooms' => bedrooms.to_i,
      'area' => area.to_i,
      'rent' => price.to_i,
      'zip_code_code' => postnum.to_i,
      'property_type' => property_type,
      'images' => imgs.join(','),
      'description' => description,
      'external_property' => ext
  }
end

c = Curl::Easy.new(URL)
c.useragent = UA
# FIXME: Check HTTP GET result
c.http_get
doc = Nokogiri::HTML(c.body_str)
c.close


rentals = doc.xpath('//div[@class="reditem_text_name"]/a').map do |node_a|
  href = URL_PREFIX + node_a['href']
  puts href
  sleep SLEEP
  parse_final_page(href)
end

###

rentals.flatten!

filename = "yaml/"+File.basename(__FILE__, ".rb")+'.yaml'
if File.exist? filename+"_old" then
  File.delete filename+"_old"
end
if File.exist? filename then
  File.rename filename, filename+"_old"
end
File.open(filename, 'w') do |out|
  out.write(YAML::dump(rentals))
end
