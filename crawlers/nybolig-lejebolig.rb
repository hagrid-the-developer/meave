require 'curb'
require 'nokogiri'
require 'cgi'
require 'yaml'


URL_PREFIX = 'http://nybolig-lejebolig.dk'
URL_POST = URL_PREFIX + '/index.php?option=com_nbudlejning&task=dosearch'
URL_GET = URL_PREFIX + '/index.php?option=com_nbudlejning&task=viewresults&sortorder=postnr&page=1'
UA = 'Mozilla/4.0 (compatible; MSIE 4.5; Mac_PowerPC)'

COOKIE_FILE = '/tmp/nybolig-lejebolig.cookies.jar'

POST_STRING = 'option=com_nbudlejning&task=dosearch&sort=postnr&page=1&kommuner%5B%5D=Albertslund+Kommune&kommuner%5B%5D=Aller%F8d+Kommune&kommuner%5B%5D=Assens+Kommune&kommuner%5B%5D=Ballerup+Kommune&kommuner%5B%5D=Billund+Kommune&kommuner%5B%5D=Bornholm+Kommune&kommuner%5B%5D=Br%F8ndby+Kommune&kommuner%5B%5D=Br%F8nderslev-Dronninglund+Kommune&kommuner%5B%5D=Drag%F8r+Kommune&kommuner%5B%5D=Egedal+Kommune&kommuner%5B%5D=Esbjerg+Kommune&kommuner%5B%5D=Fan%F8+Kommune&kommuner%5B%5D=Favrskov+Kommune&kommuner%5B%5D=Faxe+Kommune&kommuner%5B%5D=Fredensborg+Kommune&kommuner%5B%5D=Fredericia+Kommune&kommuner%5B%5D=Frederiksberg+Kommune&kommuner%5B%5D=Frederikshavn+Kommune&kommuner%5B%5D=Frederikssund+Kommune&kommuner%5B%5D=Fures%F8+Kommune&kommuner%5B%5D=Faaborg-Midtfyn+Kommune&kommuner%5B%5D=Gentofte+Kommune&kommuner%5B%5D=Gladsaxe+Kommune&kommuner%5B%5D=Glostrup+Kommune&kommuner%5B%5D=Greve+Kommune&kommuner%5B%5D=Gribskov+Kommune&kommuner%5B%5D=Guldborgsund+Kommune&kommuner%5B%5D=Haderslev+Kommune&kommuner%5B%5D=Halsn%E6s+Kommune&kommuner%5B%5D=Hedensted+Kommune&kommuner%5B%5D=Helsing%F8r+Kommune&kommuner%5B%5D=Herlev+Kommune&kommuner%5B%5D=Herning+Kommune&kommuner%5B%5D=Hiller%F8d+Kommune&kommuner%5B%5D=Hj%F8rring+Kommune&kommuner%5B%5D=Holb%E6k+Kommune&kommuner%5B%5D=Holstebro+Kommune&kommuner%5B%5D=Horsens+Kommune&kommuner%5B%5D=Hvidovre+Kommune&kommuner%5B%5D=H%F8je-Taastrup+Kommune&kommuner%5B%5D=H%F8rsholm+Kommune&kommuner%5B%5D=Ikast-Brande+Kommune&kommuner%5B%5D=Ish%F8j+Kommune&kommuner%5B%5D=Jammerbugt+Kommune&kommuner%5B%5D=Kalundborg+Kommune&kommuner%5B%5D=Kerteminde+Kommune&kommuner%5B%5D=Kolding+Kommune&kommuner%5B%5D=K%F8benhavns+Kommune&kommuner%5B%5D=K%F8ge+Kommune&kommuner%5B%5D=Langeland+Kommune&kommuner%5B%5D=Lejre+Kommune&kommuner%5B%5D=Lemvig+Kommune&kommuner%5B%5D=Lolland+Kommune&kommuner%5B%5D=Lyngby-Taarb%E6k+Kommune&kommuner%5B%5D=L%E6s%F8+Kommune&kommuner%5B%5D=Mariagerfjord+Kommune&kommuner%5B%5D=Middelfart+Kommune&kommuner%5B%5D=Mors%F8+Kommune&kommuner%5B%5D=Norddjurs+Kommune&kommuner%5B%5D=Nordfyns+Kommune&kommuner%5B%5D=Nyborg+Kommune&kommuner%5B%5D=N%E6stved+Kommune&kommuner%5B%5D=Odder+Kommune&kommuner%5B%5D=Odense+Kommune&kommuner%5B%5D=Odsherred+Kommune&kommuner%5B%5D=Randers+Kommune&kommuner%5B%5D=Rebild+Kommune&kommuner%5B%5D=Ringk%F8bing-Skjern+Kommune&kommuner%5B%5D=Ringsted+Kommune&kommuner%5B%5D=Roskilde+Kommune&kommuner%5B%5D=Rudersdal+Kommune&kommuner%5B%5D=R%F8dovre+Kommune&kommuner%5B%5D=Sams%F8+Kommune&kommuner%5B%5D=Silkeborg+Kommune&kommuner%5B%5D=Skanderborg+Kommune&kommuner%5B%5D=Skive+Kommune&kommuner%5B%5D=Slagelse+Kommune&kommuner%5B%5D=Solr%F8d+Kommune&kommuner%5B%5D=Sor%F8+Kommune&kommuner%5B%5D=Stevns+Kommune&kommuner%5B%5D=Struer+Kommune&kommuner%5B%5D=Svendborg+Kommune&kommuner%5B%5D=Syddjurs+Kommune&kommuner%5B%5D=S%F8nderborg+Kommune&kommuner%5B%5D=Thisted+Kommune&kommuner%5B%5D=T%F8nder+Kommune&kommuner%5B%5D=T%E5rnby+Kommune&kommuner%5B%5D=Vallensb%E6k+Kommune&kommuner%5B%5D=Varde+Kommune&kommuner%5B%5D=Vejen+Kommune&kommuner%5B%5D=Vejle+Kommune&kommuner%5B%5D=Vesthimmerlands+Kommune&kommuner%5B%5D=Viborg+Kommune&kommuner%5B%5D=Vordingborg+Kommune&kommuner%5B%5D=%C6r%F8+Kommune&kommuner%5B%5D=Aabenraa+Kommune&kommuner%5B%5D=Aalborg+Kommune&kommuner%5B%5D=%C5rhus+Kommune&vll=on&rntmin=&rntmax=&rmsmin=&rmsmax=&armin=&armax=&dtmin='

def parse_price(price_str)
    price_str.match(%r{\d+([.]\d+)*(,\d+)?}) { |m| return m[0].gsub(%r{[.]}, '').gsub(%r{,}, '.').to_f }
    return nil
end

def parse_postnum(addr)
    addr.match(%r{\d{4}}) { |m| return m[0] }
    return nil
end

def load_images(url)
    c = Curl::Easy.new(url)
    c.useragent = UA
    # FIXME: Check HTTP GET result
    c.http_get
    imgs = c.body_str.scan(%r{http:[/][/]mis16.mindworking.dk[/][^?]*})
    c.close
    return imgs.uniq
end

def parse_node(node)
    address = node.at_xpath('./td[3]').text.strip.gsub(%r{\s*\n\s*}, ', ')
    postnum = parse_postnum address
    url = URL_PREFIX + '/' + node.at_xpath('./td[3]/b/a')['href'].strip
    price = parse_price node.at_xpath('./td[11]').text
    imgs = load_images(url)

    return {
        'url' => url,
        'title' => address,
        'address' => address,
        'price' => price,
        'zip' => postnum,
        'imgs' => imgs.join(','),
    }
end

c_post = Curl::Easy.new(URL_POST)
c_post.useragent = UA
c_post.enable_cookies = true
c_post.cookiejar = COOKIE_FILE
# FIXME: Check HTTP POST result
c_post.http_post(POST_STRING)
c_post.close

c_get = Curl::Easy.new(URL_GET)
c_get.enable_cookies = true
c_get.cookiefile = COOKIE_FILE
c_get.useragent = UA
# FIXME: Check HTTP GET result
c_get.http_get
doc = Nokogiri::HTML(c_get.body_str)
c_get.close

rentals = doc.xpath('//div[@id="Container_Content"]/div[@class="whitepagesection"]/table/tr[@valign="top" and @align="left"]').map do |n|
    parse_node(n)
end

###

rentals.flatten!

File.open('test.yaml', 'w') do |out|
  out.write(YAML::dump(rentals))
end
