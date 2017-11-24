//
// Response.cc
//

#include "Response.hh"

using namespace WWW;

using namespace HTTP;

static const char *Reason(Response::Codes code) {
   switch (code) {
   case Response::Continue :
      return "Continue";
   case Response::SwitchingProtocols :
      return "Switching Protocols";
   case Response::Processing :
      return "Processing";

   case Response::OK :
      return "OK";
   case Response::Created :
      return "Created";
   case Response::Accepted :
      return "Accepted";
   case Response::NonAuthoritative :
      return "Non-Authoritative Information";
   case Response::NoContent :
      return "No Content";
   case Response::ResetContent :
      return "Reset Content";
   case Response::PartialContent :
      return "Partial Content";
   case Response::MultiStatus :
      return "Multi-Status";
   case Response::AlreadyReported :
      return "Already Reported";
   case Response::IMUsed :
      return "IM Used";

   case Response::MultipleChoices :
      return "Multiple Choices";
   case Response::MovedPermanently :
      return "Moved Permanently";
   case Response::Found :
      return "Found";
   case Response::SeeOther :
      return "See Other";
   case Response::NotModifed :
      return "Not Modifed";
   case Response::UseProxy :
      return "Use Proxy";
   case Response::SwitchProxy :
      return "Switch Proxy";
   case Response::TemporaryRedirect :
      return "Temporary Redirect";
   case Response::PermanentRedirect :
      return "Permanent Redirect";

   case Response::BadRequest :
      return "Bad Request";
   case Response::Unauthorized :
      return "Unauthorised";
   case Response::PaymentRequired :
      return "Payment Required";
   case Response::Forbidden :
      return "Forbidden";
   case Response::NotFound :
      return "Not Found";
   case Response::MethodNotAllowed :
      return "Method Not Allowed";
   case Response::NotAcceptable :
      return "Not Acceptable";
   case Response::ProxyAuthenticationRequired :
      return "Proxy Authentication Required";
   case Response::RequestTimeout :
      return "Request Timeout";
   case Response::Conflict :
      return "Conflict";
   case Response::Gone :
      return "Gone";
   case Response::LengthRequired :
      return "Length Required";
   case Response::PreconditionFailed :
      return "Precondition Failed";
   case Response::PayloadTooLarge :
      return "Payload Too Large";
   case Response::URITooLong :
      return "URI Too Long";
   case Response::UnsupportedMediaType :
      return "Unsupported Media Type";
   case Response::RangeNotSatisifiable :
      return "Range Not Satisfiable";
   case Response::ExpectationFailed :
      return "Expectation Failed";
   case Response::ImATeapot :
      return "I'm a Teapot";
   case Response::MisdirectedRequest :
      return "Misdirected Request";
   case Response::UnprocessableEntity :
      return "Unprocessable Entity";
   case Response::Locked :
      return "Locked";
   case Response::FailedDependency :
      return "Failed Dependency";
   case Response::UpgradeRequired :
      return "Upgrade Required";
   case Response::PreconditionRequired :
      return "Precondition Required";
   case Response::TooManyRequests :
      return "Too Many Requests";
   case Response::RequestHeaderFieldsTooLarge :
      return "Request Header Fields Too Large";
   case Response::UnavailableForLegalReasons :
      return "Unavailable For Legal Reasons";

   case Response::InternalServerError :
      return "Internal Server Error";
   case Response::NotImplemented :
      return "Not Implemented";
   case Response::BadGateway :
      return "Bad Gateway";
   case Response::ServiceUnavailable :
      return "Service Unavailable";
   case Response::GatewayTimeout :
      return "Gateway Timeout";
   case Response::HTTPVersionNotSupported :
      return "HTTP Version Not Supported";
   case Response::VariantAlsoNegotiates :
      return "Variant Also Negotiates";
   case Response::InsufficientStorage :
      return "Insufficient Storage";
   case Response::LoopDetected :
      return "Loop Detected";
   case Response::NotExtended :
      return "Not Extended";
   case Response::NetworkAuthenticationRequired :
      return "Network Authentication Required";
   } // switch

   // Unknown Code
   switch (Response::Category(code)) {
   case Response::Informational :
      return "Unknown Informational Response";
   case Response::Success :
      return "Unknown Success Response";
   case Response::Redirection :
      return "Unknown Redirection Response";
   case Response::ClientError :
      return "Unknown Client Error Response";
   case Response::ServerError :
      return "Unknown Server Error Response";
   default :
      // Completely unknown code!
      return "Unknown Response";
   } // switch
}; // Reason(Codes)

static String Encode(HTTP::Versions version, Response::Codes code) {
   String s = Version(version);
   s += ' ';
   s += code;
   s += ' ';
   s += Reason(code);
   return s;
} // Encode(version, code)

Response::Categories Response::Category(Codes code) {
   return (Categories)(code / 100 * 100);
} // Response::Categroy(code)

Response::Response(Versions version, Codes code, Size length) :
          Response(version,code) {
   Add(ContentLength, length);
} // Response::Response(version, code, length)

Response::Response(Versions version, Codes code, const String &body) :
          response(Encode(version,code)),
          headers(),
          body(body) {
   time_t now = ::time(0);

   tm gmt;
   ::gmtime_r(&now,&gmt);

   char buf[32];
   ::strftime(buf, sizeof buf, "%a, %d %b %Y %H:%M:%S %Z", &gmt);
   Add("Date", buf);
} // Response::Response(version, code, body)

void Response::Add(const String &key, Set &values) {
   headers[key] = values;
} // Response::Add(key,values)

void Response::Add(const String &key, int value) {
   Add(key, std::to_string(value));
} // Response::Add(key, value)

void Response::Add(const String &key, unsigned value) {
   Add(key, std::to_string(value));
} // Response::Add(key, value)

void Response::Add(const String &key, const String &field, int value) {
   Add(key, field+std::to_string(value));
} // Response::Add(key, field, value)

void Response::Add(const String &key, const String &field, unsigned value) {
   Add(key, field+std::to_string(value));
} // Response::Add(key, field, value)

void Response::Add(const String &key, const String &value) {
   auto i = headers.find(key);
   if (i==headers.end()) {
      Set set;
      set.insert(value);
      headers[key] = set;
   } // if
   else {
      i->second.insert(value);
   } // else
} // Response::Add(key,value)

void Response::Append(const String &lines) {
   body += lines;
} // Response::Append(lines)

Response::operator String() const {
   String s;
   s.reserve(2048);

   s = response + HTTP::EOL;
   for (const auto &h : headers) {
      s += h.first;
      s += ":";

      char sep = ' ';
      for (const auto &i : h.second) {
         s += sep;
         s += i;
         sep = ',';
      } // for
      s += HTTP::EOL;
   } // for

   if (headers.find(ContentLength)==headers.end()) {
      s += ContentLength;
      s += ": ";
      s += body.length();
      s += HTTP::EOL;
   } // if

   s += HTTP::EOL; // Blank line between header and body
   s += body;
   return s;
} // Response::operator String()
