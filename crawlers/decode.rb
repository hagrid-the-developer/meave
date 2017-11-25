require 'cgi'

ARGV.each do |arg|
    dec_arg = CGI::unescape(arg)
    puts "\"#{dec_arg}\""
end
