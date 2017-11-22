//
// Response.cc
//

#include "Response.hh"

using namespace WWW::HTTP;

static const char *Reason(Response::Informationals informational) {
   switch (informational) {
   case Response::Continue :
      return "Continue";
   case Response::SwitchingProtocols :
      return "Switching Protocols";
   case Response::Processing :
      return "Processing";
   } // switch
   return "Unknown Informational";
} // Reason(Informationals)

static const char *Reason(Response::Successes success) {
   switch (success) {
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
   } // switch
   return "Unknown Success";
} // Reason(Successes)

static const char *Reason(Response::Redirections redirection) {
   switch (redirection) {
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
   } // switch
   return "Unknown Redirection";
} // Reason(Redirections)

static const char *Reason(Response::ClientErrors clientError) {
   switch (clientError) {
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
   } // switch
   return "Unknown Client Error";
}; // Reason(ClientErrors)

static const char *Reason(Response::ServerErrors serverError) {
   switch (serverError) {
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
   return "Unknown Server Error";
}; // Reason(ServerErrors)

template <typename Code>
const char *ReasonString(Code code) {
   return Reason(code);
} // ReasonString(Code)
