require 'curb'
require 'nokogiri'
require 'cgi'
require 'yaml'
require 'digest/md5'

URL_PREFIX = 'http://hdejendomme.dk'
URL = URL_PREFIX + '/search/form/2?countrypart=1&minz=0000&maxz=9999&areaname=&hustype_lejlighed=1&hustype_raekkehus=1&hustype_fritliggende=1&hustype_tvillingehus=1&mins=&maxs=&minr=&maxr=&minp=&maxp=&husdyr=2'
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
    Digest::MD5.hexdigest(url).to_i(16)
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

  title = doc.at_css('div.area_top > p').text.strip #doc.at_xpath('//span[contains(@id, "_Headline")]').text.strip
  addr1 = doc.at_css('div.area_top > h3').text.strip
  postnum = parse_postnum addr1
  price = nil #parse_price doc.at_xpath('//span[contains(@id, "_Rent")]').text.strip
  description = doc.css('.area_text > p').to_a.map{ |item| item.text.strip }.delete_if{ |item| item.match %r{^\p{Z}*$}u }.join '; '
  property_type = nil #title.match(%r{ hus }) ? doc.at_xpath('//span[contains(@id, "_ResidenceType")]').text.strip : 'Apartment';
  imgs = doc.xpath('//div[@class = "area_imgs"]//img').map { |img| img['src'].strip.sub(%r{[/]areasmall}i, '/area') }
  imgs.uniq!


  return (doc.css('.type') + doc.css('.type_active')).to_a.map do |node|
        area = node.at_css('a.typelink').text.match(%r{(\d+)\s+m}) { |m| m[1].to_i }
        bedrooms = node.at_css('a.typelink').text.match(%r{\d+\D+(\d+)\s}) { |m| m[1].to_i - 1 }
        price = node.at_css('a.typelink').text.match(%r{fra\s+(\d+(?:[.]\d+)?)\s+kr/md}) { |m| parse_price m[1] }
        imgs_node = node.xpath('//div[@class = "type_imgs"]//img').map { |img| img['src'].strip.sub(%r{[/]areatiny}i, '/area') }
        imgs_node.uniq!

        node.xpath('.//p[ not(contains(@style, "line-through")) ]/text()').to_a.delete_if { |n| not n.text.match %r{\d+} }.map do |n|
            addr0 = n.text.strip
            address = addr0 + '; ' + addr1
            streetname = addr0.match(/^[^0-9]+/) { |m| m[0].strip }
            ext = {
                'external_property_id' => parse_external_property_id("#{url}\t#{address}"),
                'provider_id' => PROVIDER_ID,
                'property_url' => url,
            }

            {
                'heading' => title,
                'address' => address,
                'street_name' => streetname,
                'bedrooms' => bedrooms,
                'area' => area,
                'rent' => price,
                'zip_code_code' => postnum.to_i,
                'property_type' => property_type,
                'images' => (imgs + imgs_node).join(','),
                'description' => description,
                'external_property' => ext
           }
        end
    end
end


rentals = []
url = URL
c = Curl::Easy.new(url)
c.useragent = UA
# FIXME: Check HTTP GET result
c.http_get
doc = Nokogiri::HTML(c.body_str)
c.close

rentals = doc.css('a.read_more').to_a.shuffle.map do |node_a|
    href = node_a['href']
    puts "\t#{href}"
#    sleep calc_sleep_time
    begin
        parse_final_page(href)
    rescue NoMethodError => e
        puts "\t\texcpetion: #{e}"
        nil
    end
end

###

rentals.flatten!
puts "rentals=#{rentals}"

filename = "yaml/" + File.basename(__FILE__, ".rb") + '.yaml'
if File.exist? filename+"_old" then
  File.delete filename+"_old"
end
if File.exist? filename then
  File.rename filename, filename+"_old"
end
File.open(filename, 'w') do |out|
  out.write(YAML::dump(rentals))
end
