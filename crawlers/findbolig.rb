require 'curb'
require 'nokogiri'
require 'cgi'
require 'yaml'


URL_PREFIX = 'http://www.findbolig.nu'
URL = URL_PREFIX + '/ledigeboliger/liste.aspx?&showrented=1&showyouth=1&showlimitedperiod=1&showunlimitedperiod=1&showOpenDay=0&page=1&pagesize=100'
UA = 'Mozilla/4.0 (compatible; MSIE 4.5; Mac_PowerPC)'
PROVIDER_ID = 8

def calc_sleep_time
    return 0 if (dice = rand) < 0.1
    val = rand
    return val if dice < 0.6
    return 4.0*val if dice < 0.9
    return 10.0*val
end

def parse_price(price_str)
  price_str.match(%r{\d+([.]\d+)*(,\d+)?}) { |m| return m[0].gsub(%r{[.]}, '').gsub(%r{,}, '.').to_f }
  return nil
end

def parse_postnum(addr)
  addr.match(%r{\d{4}}) { |m| return m[0] }
  return nil
end

def parse_external_property_id(url)
  url.match(/aid=([0-9]+)/)[1]
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

  title = doc.at_xpath('//span[contains(@id, "_Headline")]').text.strip
  addr0 = doc.at_css('#ctl00_placeholdercontent_0_lbl_Address').text.strip
  addr1 = doc.at_css('#ctl00_placeholdercontent_0_lbl_Location').text.strip
  streetname = addr0.match(/^[^0-9]+/)[0].strip
  postnum = parse_postnum addr1
  address = addr0 + '; ' + addr1
  price = parse_price doc.at_xpath('//span[contains(@id, "_Rent")]').text.strip
  description = title + '; ' + doc.at_xpath('//span[contains(@id, "_Description")]').text.strip
  area = doc.at_xpath('//span[contains(@id, "_Area")]').text.to_i
  property_type = title.match(%r{ hus }) ? doc.at_xpath('//span[contains(@id, "_ResidenceType")]').text.strip : 'Apartment';
  bedrooms = doc.at_xpath('//span[contains(@id, "_lbl_Rooms")]').text.to_i - 1
  imgs = doc.xpath('//div[contains(@id, "_pnl_Thumbnails")]/li/img').map { |img| URL_PREFIX + img['id'].strip.sub(%r{_\d+x\d+[.]jpg}i, '.jpg') }
  imgs.uniq!
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
      'property_type' => property_type,
      'images' => imgs.join(','),
      'description' => description,
      'external_property' => ext
  }
end

rentals = []
rents = nil
for page_num in 1..100 do
  url = URL
  url.sub! %r{page=\d+}, "page=#{page_num}"
  puts "#{url}"
  c = Curl::Easy.new(url)
  c.useragent = UA
  # FIXME: Check HTTP GET result
  c.http_get
  doc = Nokogiri::HTML(c.body_str)
  c.close


  rents = doc.css('td.imgCol > a.advertLink').to_a.shuffle.map do |node_a|
    href = URL_PREFIX + node_a['href']
    # Skip the redirect
    href.sub!(%r{^http://www.findbolig.nu/bolig.aspx[?]}, 'http://www.findbolig.nu/Findbolig-nu/Find%20bolig/Ledige%20boliger/Boligpraesentation/Boligen.aspx?')
    puts "\t#{href}"
    sleep calc_sleep_time
    parse_final_page(href)
  end
  break if rents.to_a.empty?
  rentals += rents
end

###

rentals.flatten!
puts "rentals=#{rentals}"

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
