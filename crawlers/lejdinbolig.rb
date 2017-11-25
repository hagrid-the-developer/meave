require 'curb'
require 'nokogiri'
require 'cgi'
require 'yaml'


URL_PREFIX = 'http://lejdinbolig.nu'
URL = URL_PREFIX + '/index.php?action=residence_overview'
UA = 'Mozilla/4.0 (compatible; MSIE 4.5; Mac_PowerPC)'
SLEEP = 1.0
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
  url.match(/residence_id=([0-9]+)/)[1]
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

  title = doc.at_xpath('//tr/td[@class="ResidenceDetailsRightHead" and contains(text(), "Property")]/following::tr[1]/td//text()').text.strip
  address = doc.xpath('//tr/td[@class="ResidenceDetailsRightHead" and contains(text(), "Adress")]/following::tr[1]/td//text()').map{ |t| t.content.strip }.join ' '
  streetname = address.match(/^[^0-9]+/)[0].strip
  postnum = parse_postnum address
  price = parse_price doc.xpath('//tr/td[@class="ResidenceDetailsRightHead" and contains(text(), "Rent")]/following::tr[1]/td//text()').map {|n| n.content.strip}.join ' '
  imgs = doc.xpath('//img[contains(@src, "plan_container/")]').map { |img| URL_PREFIX + '/' + img['src'].strip } +
         doc.xpath('//a[contains(@href, "&show_pic=")]').map { |a| URL_PREFIX + '/' + a['href'].match('&show_pic=([^&\s]+)')[1] }
  imgs.uniq!
  area = doc.at_xpath('//a[contains(@href, "brutto_areal")]').text.to_i
  bedrooms = doc.xpath('//tr/td[@class="ResidenceDetailsRightHead" and contains(text(), "Dwelling info")]/following::tr[1]/td/a[contains(@href, "antal_vearelser")]').to_a.first(1).map do |n|
    rooms_str = n.text.match(%r{(\d+|-)\s+room})[1]
    if rooms_str == '-'
      nil
    else
      rooms_num = rooms_str.to_i
      rooms_num > 0 ? rooms_num - 1 : nil
    end
  end.first
  ext = {
      'external_property_id' => parse_external_property_id(url).to_i,
      'provider_id' => PROVIDER_ID,
      'property_url' => url,
  }

  return {
      'heading' => title,
      'address' => address,
      'street_name' => streetname,
      'bedrooms' => bedrooms,
      'area' => area,
      'rent' => price,
      'zip_code_code' => postnum.to_i,
      'property_type' => 'Apartment',
      'images' => imgs.join(','),
      'description' => nil,
      'external_property' => ext
  }
end

c = Curl::Easy.new(URL)
c.useragent = UA
# FIXME: Check HTTP GET result
c.http_get
doc = Nokogiri::HTML(c.body_str)
c.close


rentals = doc.xpath('//tr[@class="ResidenceOverviewRow"]').map do |node_tr|
  loc_enc = %r{'([^']+)'}.match(node_tr['onclick'])[1]
  loc = CGI::unescape loc_enc
  href = URL_PREFIX + '/' + loc
  status = node_tr.at_xpath('./td[last()]').text.strip
  if status == 'Vacant'
    puts href
    sleep SLEEP
    parse_final_page(href)
  else
    puts "Skipping #{href}, status: #{status}"
    []
  end
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
