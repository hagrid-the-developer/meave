require 'curb'
require 'nokogiri'
require 'cgi'
require 'yaml'


URL_PREFIX = "http://www.domea.dk"
URL = URL_PREFIX + "/sog-bolig/Ledige-boliger/Sider/default.aspx"
UA = "Mozilla/4.0 (compatible; MSIE 4.5; Mac_PowerPC)"
PROVIDER_ID = 1234567890

###

#def get_callbackparam(index)
#    index < 10 ? "c0%3AFR%7C1%3B0%3BGB%7C20%3B14%7CCUSTOMCALLBACK1%7C#{index}%3B" : 
#                 "c0%3AFR%7C1%3B0%3BGB%7C21%3B14%7CCUSTOMCALLBACK2%7C#{index}%3B"
#end

def calc_sleep_time
    return 0 if (dice = rand) < 0.1
    val = rand
    return val if dice < 0.6
    return 4.0*val if dice < 0.9
    return 10.0*val
end

def download_url(url)
    count = 0
    str = nil
    begin
        sleep calc_sleep_time

        Curl::Easy.new(url) do |curl|
            curl.useragent = UA
            curl.http_get
            return curl.body_str
        end
    rescue Exception => e
        count > 5 and raise e
        count += 1
        retry
    end
end

def get_postquery(index, request_digest, event_validation)
    "MSOWebPartPage_PostbackSource=&MSOTlPn_SelectedWpId=&MSOTlPn_View=0&MSOTlPn_ShowSettings=False&MSOGallery_SelectedLibrary=&MSOGallery_FilterString=&MSOTlPn_Button=none&__EVENTTARGET=&__EVENTARGUMENT=&__REQUESTDIGEST=#{CGI::escape(request_digest)}&MSOSPWebPartManager_ExitingDesignMode=false&MSOWebPartPage_Shared=&MSOLayout_LayoutChanges=&MSOLayout_InDesignMode=&MSOSPWebPartManager_OldDisplayModeName=Browse&MSOSPWebPartManager_StartWebPartEditingName=false&MSOSPWebPartManager_EndWebPartEditing=false&__VIEWSTATE=%2FwEPDwUJOTk4ODk4NDI1D2QWAmYPZBYCAgEPZBYEAgEPZBYCAgcPZBYCZg9kFgICAw8WAh4TUHJldmlvdXNDb250cm9sTW9kZQspiAFNaWNyb3NvZnQuU2hhcmVQb2ludC5XZWJDb250cm9scy5TUENvbnRyb2xNb2RlLCBNaWNyb3NvZnQuU2hhcmVQb2ludCwgVmVyc2lvbj0xNC4wLjAuMCwgQ3VsdHVyZT1uZXV0cmFsLCBQdWJsaWNLZXlUb2tlbj03MWU5YmNlMTExZTk0MjljAWQCAw8WAh4FY2xhc3MFHmRQbERldGFpbFJlbnRhbFZpZXdXZWJQYXJ0UGFnZRYCAgEPZBYEAgMPFgIeEkVuYWJsZUNsaWVudFNjcmlwdGgWBAUmZ19hN2FmYzRhZV9mZDM4XzRjMTZfYWRiNl9kOTBiZjU0ODI3MzAQZGQWAmYPZBYEZg8PFgIeBFRleHQFBFPDuGdkZAIDDxYCHglpbm5lcmh0bWwF0Q48ZGl2IGNsYXNzPSJyb3RhdG9yIiBzdHlsZT0ibWFyZ2luOjBweDsgZmxvYXQ6bGVmdDsiPg0KPHVsPg0KPGxpIGNsYXNzPSJzaG93IiBzdHlsZT0iYm9yZGVyOm5vbmUhaW1wb3J0YW50O2JhY2tncm91bmQtY29sb3I6I0I2QjZCNiI%2BDQo8ZGl2IGNsYXNzPSJ0Ij48c3BhbiBjbGFzcz0idGwiPjwvc3Bhbj48c3BhbiBjbGFzcz0idHIiPjwvc3Bhbj48L2Rpdj48ZGl2IGNsYXNzPSJjbCI%2BPGRpdiBzdHlsZT0iYmFja2dyb3VuZC1jb2xvcjpibHVlIiBjbGFzcz0iY3IiPg0KPGEgaHJlZj0iIiB0aXRsZT0iTsOmcmhlZCBpIHRyeWdnZSByYW1tZXIiPjxpbWcgc3JjPSIvc29nLWJvbGlnL2xlZGlnZS1ib2xpZ2VyL2thbXBhZ25lci9rYW1wYWduZV8wMDdfZmFtaWxpZS5qcGciIGNsYXNzPSJJbWFnZVJvdGF0aW5nIiBhbHQ9Ik7DpnJoZWQgaSB0cnlnZ2UgcmFtbWVyIiAvPjwvYT4NCjwvZGl2PjwvZGl2PjxkaXYgc3R5bGU9ImJhY2tncm91bmQtaW1hZ2U6bm9uZSFpbXBvcnRhbnQiIGNsYXNzPSJiIj48c3BhbiBzdHlsZT0iIGJhY2tncm91bmQtcG9zaXRpb246IC0yMXB4IC02MHB4IiBjbGFzcz0iYmwiPjwvc3Bhbj48c3BhbiBzdHlsZT0iYmFja2dyb3VuZC1wb3NpdGlvbjogLTI0cHggLTYwcHg7IiBjbGFzcz0iYnIiPjwvc3Bhbj48L2Rpdj4NCjwvbGk%2BDQo8bGkgY2xhc3M9InNob3ciIHN0eWxlPSJib3JkZXI6bm9uZSFpbXBvcnRhbnQ7YmFja2dyb3VuZC1jb2xvcjojQjZCNkI2Ij4NCjxkaXYgY2xhc3M9InQiPjxzcGFuIGNsYXNzPSJ0bCI%2BPC9zcGFuPjxzcGFuIGNsYXNzPSJ0ciI%2BPC9zcGFuPjwvZGl2PjxkaXYgY2xhc3M9ImNsIj48ZGl2IHN0eWxlPSJiYWNrZ3JvdW5kLWNvbG9yOmJsdWUiIGNsYXNzPSJjciI%2BDQo8YSBocmVmPSIiIHRpdGxlPSJOw6ZyaGVkIGkgdHJ5Z2dlIHJhbW1lciI%2BPGltZyBzcmM9Ii9zb2ctYm9saWcvbGVkaWdlLWJvbGlnZXIva2FtcGFnbmVyL2thbXBhZ25lXzAxOF90cmVodXNlLmpwZyIgY2xhc3M9IkltYWdlUm90YXRpbmciIGFsdD0iTsOmcmhlZCBpIHRyeWdnZSByYW1tZXIiIC8%2BPC9hPg0KPC9kaXY%2BPC9kaXY%2BPGRpdiBzdHlsZT0iYmFja2dyb3VuZC1pbWFnZTpub25lIWltcG9ydGFudCIgY2xhc3M9ImIiPjxzcGFuIHN0eWxlPSIgYmFja2dyb3VuZC1wb3NpdGlvbjogLTIxcHggLTYwcHgiIGNsYXNzPSJibCI%2BPC9zcGFuPjxzcGFuIHN0eWxlPSJiYWNrZ3JvdW5kLXBvc2l0aW9uOiAtMjRweCAtNjBweDsiIGNsYXNzPSJiciI%2BPC9zcGFuPjwvZGl2Pg0KPC9saT4NCjxsaSBjbGFzcz0ic2hvdyIgc3R5bGU9ImJvcmRlcjpub25lIWltcG9ydGFudDtiYWNrZ3JvdW5kLWNvbG9yOiNCNkI2QjYiPg0KPGRpdiBjbGFzcz0idCI%2BPHNwYW4gY2xhc3M9InRsIj48L3NwYW4%2BPHNwYW4gY2xhc3M9InRyIj48L3NwYW4%2BPC9kaXY%2BPGRpdiBjbGFzcz0iY2wiPjxkaXYgc3R5bGU9ImJhY2tncm91bmQtY29sb3I6Ymx1ZSIgY2xhc3M9ImNyIj4NCjxhIGhyZWY9IiIgdGl0bGU9Ik7DpnJoZWQgaSB0cnlnZ2UgcmFtbWVyIj48aW1nIHNyYz0iL3NvZy1ib2xpZy9sZWRpZ2UtYm9saWdlci9rYW1wYWduZXIva2FtcGFnbmVfMDc2X2tpZC1lbnRyZXByZW5ldXIuanBnIiBjbGFzcz0iSW1hZ2VSb3RhdGluZyIgYWx0PSJOw6ZyaGVkIGkgdHJ5Z2dlIHJhbW1lciIgLz48L2E%2BDQo8L2Rpdj48L2Rpdj48ZGl2IHN0eWxlPSJiYWNrZ3JvdW5kLWltYWdlOm5vbmUhaW1wb3J0YW50IiBjbGFzcz0iYiI%2BPHNwYW4gc3R5bGU9IiBiYWNrZ3JvdW5kLXBvc2l0aW9uOiAtMjFweCAtNjBweCIgY2xhc3M9ImJsIj48L3NwYW4%2BPHNwYW4gc3R5bGU9ImJhY2tncm91bmQtcG9zaXRpb246IC0yNHB4IC02MHB4OyIgY2xhc3M9ImJyIj48L3NwYW4%2BPC9kaXY%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%2BPGxpPk9sZGVuYnVyZyBBbGzDqSAzPC9saT48bGk%2BMjYzMCBUYWFzdHJ1cDwvbGk%2BPGxpPkVtYWlsOiA8YSBocmVmPSJtYWlsdG86ZG9tZWFAZG9tZWEuZGsiPmRvbWVhQGRvbWVhLmRrPC9hPjwvbGk%2BPGxpPlRsZjogKzQ1IDc2IDY0IDY0IDY0PC9saT48L3VsPg0KZAICD2QWBAIBDxYCHwMFCUVuIGRlbCBhZmQCAw8WBB4EaHJlZgUTaHR0cDovL3d3dy5kb21lYS5kax4Fc3R5bGUFqgFiYWNrZ3JvdW5kOnRyYW5zcGFyZW50IHVybCgnL19sYXlvdXRzL0RvbWVhLnNoYXJlcG9pbnQuYnJhbmRpbmcvaW1nL2ZpeGVkLnBuZycpIG5vLXJlcGVhdCBzY3JvbGwgMCAwO2Rpc3BsYXk6YmxvY2s7aGVpZ2h0OjE5cHg7d2lkdGg6NTlweDtiYWNrZ3JvdW5kLXBvc2l0aW9uOi01OHB4IC0yM3B4O2QYAQUeX19Db250cm9sc1JlcXVpcmVQb3N0QmFja0tleV9fFhMFVWN0bDAwJHdlYlBhcnRNYW5hZ2VyJGdfYTdhZmM0YWVfZmQzOF80YzE2X2FkYjZfZDkwYmY1NDgyNzMwJGN0bDAwJHVpU2VhcmNoQXZhbGlibGVOb3cFVmN0bDAwJHdlYlBhcnRNYW5hZ2VyJGdfYTdhZmM0YWVfZmQzOF80YzE2X2FkYjZfZDkwYmY1NDgyNzMwJGN0bDAwJHVpU2VhcmNoQXZhbGlibGVMb25nBVdjdGwwMCR3ZWJQYXJ0TWFuYWdlciRnX2E3YWZjNGFlX2ZkMzhfNGMxNl9hZGI2X2Q5MGJmNTQ4MjczMCRjdGwwMCR1aVNlYXJjaEF2YWxpYmxlU2hvcnQFVWN0bDAwJHdlYlBhcnRNYW5hZ2VyJGdfYTdhZmM0YWVfZmQzOF80YzE2X2FkYjZfZDkwYmY1NDgyNzMwJGN0bDAwJHVpU2VhcmNoQXZhbGlibGVOZXcFWmN0bDAwJHdlYlBhcnRNYW5hZ2VyJGdfYTdhZmM0YWVfZmQzOF80YzE2X2FkYjZfZDkwYmY1NDgyNzMwJGN0bDAwJHVpU2VhcmNoQXZhbGlibGVFbGV2YXRvcgVZY3RsMDAkd2ViUGFydE1hbmFnZXIkZ19hN2FmYzRhZV9mZDM4XzRjMTZfYWRiNl9kOTBiZjU0ODI3MzAkY3RsMDAkdWlTZWFyY2hBdmFsaWJsZVBhcmtpbmcFV2N0bDAwJHdlYlBhcnRNYW5hZ2VyJGdfYTdhZmM0YWVfZmQzOF80YzE2X2FkYjZfZDkwYmY1NDgyNzMwJGN0bDAwJHVpU2VhcmNoQXZhbGlibGVIb3VzZQVeY3RsMDAkd2ViUGFydE1hbmFnZXIkZ19hN2FmYzRhZV9mZDM4XzRjMTZfYWRiNl9kOTBiZjU0ODI3MzAkY3RsMDAkdWlTZWFyY2hBdmFsaWJsZUhvdXNlQW5pbWFscwVcY3RsMDAkd2ViUGFydE1hbmFnZXIkZ19hN2FmYzRhZV9mZDM4XzRjMTZfYWRiNl9kOTBiZjU0ODI3MzAkY3RsMDAkdWlTZWFyY2hBdmFsaWJsZVBsYXlncm91bmQFWWN0bDAwJHdlYlBhcnRNYW5hZ2VyJGdfYTdhZmM0YWVfZmQzOF80YzE2X2FkYjZfZDkwYmY1NDgyNzMwJGN0bDAwJHVpU2VhcmNoSG91c2VUeXBlM0NoZWNrBVljdGwwMCR3ZWJQYXJ0TWFuYWdlciRnX2E3YWZjNGFlX2ZkMzhfNGMxNl9hZGI2X2Q5MGJmNTQ4MjczMCRjdGwwMCR1aVNlYXJjaEhvdXNlVHlwZTRDaGVjawVZY3RsMDAkd2ViUGFydE1hbmFnZXIkZ19hN2FmYzRhZV9mZDM4XzRjMTZfYWRiNl9kOTBiZjU0ODI3MzAkY3RsMDAkdWlTZWFyY2hIb3VzZVR5cGUxQ2hlY2sFWWN0bDAwJHdlYlBhcnRNYW5hZ2VyJGdfYTdhZmM0YWVfZmQzOF80YzE2X2FkYjZfZDkwYmY1NDgyNzMwJGN0bDAwJHVpU2VhcmNoSG91c2VUeXBlN0NoZWNrBVljdGwwMCR3ZWJQYXJ0TWFuYWdlciRnX2E3YWZjNGFlX2ZkMzhfNGMxNl9hZGI2X2Q5MGJmNTQ4MjczMCRjdGwwMCR1aVNlYXJjaEhvdXNlVHlwZTVDaGVjawVZY3RsMDAkd2ViUGFydE1hbmFnZXIkZ19hN2FmYzRhZV9mZDM4XzRjMTZfYWRiNl9kOTBiZjU0ODI3MzAkY3RsMDAkdWlTZWFyY2hIb3VzZVR5cGU2Q2hlY2sFWWN0bDAwJHdlYlBhcnRNYW5hZ2VyJGdfYTdhZmM0YWVfZmQzOF80YzE2X2FkYjZfZDkwYmY1NDgyNzMwJGN0bDAwJHVpU2VhcmNoSG91c2VUeXBlMkNoZWNrBVljdGwwMCR3ZWJQYXJ0TWFuYWdlciRnXzBhMjcxOGQ5X2Q0OTNfNGJiMl84YzFhX2NlZjdjMTRkYmI1MiRjdGwwMCR1aVJlc3VsdExpc3RWaWV3UmVudGFscwVuY3RsMDAkd2ViUGFydE1hbmFnZXIkZ18wYTI3MThkOV9kNDkzXzRiYjJfOGMxYV9jZWY3YzE0ZGJiNTIkY3RsMDAkdWlSZXN1bHRMaXN0Vmlld1JlbnRhbHMkY3RsMDAkRFhFZGl0b3IxNSREREQFdGN0bDAwJHdlYlBhcnRNYW5hZ2VyJGdfMGEyNzE4ZDlfZDQ5M180YmIyXzhjMWFfY2VmN2MxNGRiYjUyJGN0bDAwJHVpUmVzdWx0TGlzdFZpZXdSZW50YWxzJGN0bDAwJERYRWRpdG9yMTUkREREJEMkRk5QUioJXPhs7LS8qgoPmGJiW8PsyzM%3D&ctl00%24ScriptManager=&ctl00%24webPartManager%24g_a7afc4ae_fd38_4c16_adb6_d90bf5482730%24ctl00%24uiRentMinHidden=&ctl00%24webPartManager%24g_a7afc4ae_fd38_4c16_adb6_d90bf5482730%24ctl00%24uiRentMaxHidden=&ctl00%24webPartManager%24g_a7afc4ae_fd38_4c16_adb6_d90bf5482730%24ctl00%24uiSizeMinHidden=&ctl00%24webPartManager%24g_a7afc4ae_fd38_4c16_adb6_d90bf5482730%24ctl00%24uiSizeMaxHidden=&ctl00%24webPartManager%24g_a7afc4ae_fd38_4c16_adb6_d90bf5482730%24ctl00%24uiRoomMinHidden=&ctl00%24webPartManager%24g_a7afc4ae_fd38_4c16_adb6_d90bf5482730%24ctl00%24uiRoomMaxHidden=&ctl00%24webPartManager%24g_a7afc4ae_fd38_4c16_adb6_d90bf5482730%24ctl00%24uiFloorMinHidden=&ctl00%24webPartManager%24g_a7afc4ae_fd38_4c16_adb6_d90bf5482730%24ctl00%24uiFloorMaxHidden=&ctl00%24webPartManager%24g_a7afc4ae_fd38_4c16_adb6_d90bf5482730%24ctl00%24uiSearchFreeText=&ctl00%24webPartManager%24g_a7afc4ae_fd38_4c16_adb6_d90bf5482730%24ctl00%24uiSearchHouseType3Check=on&ctl00%24webPartManager%24g_a7afc4ae_fd38_4c16_adb6_d90bf5482730%24ctl00%24uiSearchHouseType4Check=on&ctl00%24webPartManager%24g_a7afc4ae_fd38_4c16_adb6_d90bf5482730%24ctl00%24uiSearchHouseType1Check=on&ctl00%24webPartManager%24g_a7afc4ae_fd38_4c16_adb6_d90bf5482730%24ctl00%24ShowMoreHidden=0&ctl00%24webPartManager%24g_0a2718d9_d493_4bb2_8c1a_cef7c14dbb52%24ctl00%24uiResultListViewRentals%24DXSelInput=&ctl00%24webPartManager%24g_0a2718d9_d493_4bb2_8c1a_cef7c14dbb52%24ctl00%24uiResultListViewRentals%24DXFocusedRowInput=0&ctl00%24webPartManager%24g_0a2718d9_d493_4bb2_8c1a_cef7c14dbb52%24ctl00%24uiResultListViewRentals%24CallbackState=BwEHAQIFU3RhdGUHkQcQBv%2F%2FAgAG%2F%2F8CAAb%2F%2FwIABv%2F%2FAgAG%2F%2F8CAAb%2F%2FwIABv%2F%2FAgAHAQIBBwICAQcDAgEHBAIBBwUCAQcGAgEHBgIBBwcCAQcIAgEHAAcABwIHAAcJBwEHAAcGBwAHAAcABwIJMDg4MjUwODA3AgxMZWRpZ0JvbGlnSWQJAgACAAMHBAIABwACAQcABwACAQcABwA%3D&__spText1=&__spText2=&_wpcmWpid=&wpcmVal=&DXScript=1_42&__CALLBACKID=ctl00%24webPartManager%24g_0a2718d9_d493_4bb2_8c1a_cef7c14dbb52%24ctl00%24uiResultListViewRentals&__CALLBACKPARAM=c0%3AFR%7C1%3B0%3BGB%7C2#{index < 10 ? 0 : 1}%3B12%7CPAGERONCLICK#{index < 10 ? 3 : 4}%7CPN#{index-1}%3B&__EVENTVALIDATION=#{CGI::escape(event_validation)}"

end

def parse_price(price_str)
    price_str.match(%r{\d+([.]\d+)*(,\d+)?}) { |m| return m[0].gsub(%r{[.]}, '').gsub(%r{,}, '.').to_f }
    return nil
end

def str_to_urls(page)
    page.scan(%r{id\s*=\s*[\\]?"(\d{9})[\\]?"}).flatten.map { |x| URL + "?ri=#{x}&DeatailsView=true" }
end

def parse_external_property_id(url)
    url.match(/ri=([0-9]+)/)[1]
end

def parse_page(url)
    doc = Nokogiri::HTML(download_url(url))

    title = doc.xpath('//span[contains(@id, "uiSalesText")]/div[1]/descendant::text()').map { |x| x.text.strip }.join ' '
    description = doc.xpath('//span[contains(@id, "uiSalesText")]/div[position() > 1]/descendant::text()').map { |x| x.text.strip }.join ' '
    address = doc.xpath('//span[contains(@id, "uiSalesAdress")]').to_a.first(1).map { |x| x.text.strip }.first
    street_name = address.match(/^[^0-9]+/) { |m| m[0].strip }
    postnum = address.match(%r{\d{4}}) { |m| m[0].to_i }
    price = doc.xpath('//td[contains(text(), "Husleje")]/following-sibling::td').to_a.first(1).map { |x| parse_price x.text.strip }.first
    property_type = doc.xpath('//td[contains(text(), "Boligtype")]/following-sibling::td').to_a.first(1).map { |x| x.text.strip }.first
    bedrooms = doc.xpath('//td[contains(text(), "relser")]/following-sibling::td').to_a.first(1).map { |x| x.text.to_i - 1 }.first
    area = doc.xpath('//td[contains(text(), "Boligareal")]/following-sibling::td').to_a.first(1).map { |x| x.text.match(%r{\d+}) { |m| m[0].to_i } }.first
    imgs_str = doc.xpath('//div[@class="dWpGalleryInner"]//img').map { |img| img['src'] }.delete_if { |str| str.match %r{no-img_THB} }.join ','

    ext = {
        'external_property_id' => parse_external_property_id(url).to_i,
        'provider_id' => PROVIDER_ID,
        'property_url' => url,
    }

    return {
        'url' => url,
        'heading' => title,
        'address' => address,
        'street_name' => street_name,
        'zip_code' => postnum,
        'rent' => price,
        'area' => area,
        'property_type' => property_type,
        'description' => description,
        'images' => imgs_str,
        'ext' => ext,
    }
end

#c = Curl::Easy.new(URL)
#c.useragent = UA
## FIXME: Check HTTP GET result
#c.http_get

#print "page=[#{c.body_str}]\n"

body_str = download_url(URL)
urls = str_to_urls(body_str)

doc = Nokogiri::HTML(body_str)
count = doc.at_css('b.dxp-summary').text.match('Side\s+\d+\s+af\s+(\d+)') { |m| m[1] }.to_i
event_validation, request_digest = doc.at_css('#__EVENTVALIDATION')['value'], doc.at_css('#__REQUESTDIGEST')['value']

puts "urls=#{urls}; count=#{count}; event_validation = #{event_validation}; request_digest = #{request_digest}"

#c.close

urls = urls + (2..count).map do |i|
    sleep calc_sleep_time
    cp = Curl::Easy.new(URL)
    cp.useragent = UA
    cp.headers['Referer'] = URL
    cp.headers['Origin'] = URL_PREFIX
    cp.http_post(get_postquery(i, request_digest, event_validation))
    us = str_to_urls(cp.body_str)
    cp.close
    puts "us=#{us}"
    us
end

urls.flatten!

rentals = urls.map do |url|
    puts "Parsing url: #{url}..."
    parse_page url
end

File.open('test.yaml', 'w') do |out|
  out.write(YAML::dump(rentals))
end
