//
// Response.hh
//

#ifndef Response_hh
#define Response_hh

#include "HTTP.hh"

class WWW::HTTP::Response {

public: // Enums

   enum Informationals {
      Informational      = 100,

      Continue           = 100,
      SwitchingProtocols = 101,
      Processing         = 102
   }; // Informationals

   enum Successes {
      Success           = 200,

      OK                = 200,
      Created           = 201,
      Accepted          = 202,
      NonAuthoritative  = 203,
      NoContent         = 204,
      ResetContent      = 205,
      PartialContent    = 206,
      MultiStatus       = 207,
      AlreadyReported   = 208,
      IMUsed            = 226
   }; // Successes

   enum Redirections {
      Redirection       = 300,

      MultipleChoices   = 300,
      MovedPermanently  = 301,
      Found             = 302,
      SeeOther          = 303,
      NotModifed        = 304,
      UseProxy          = 305,
      SwitchProxy       = 306,
      TemporaryRedirect = 307,
      PermanentRedirect = 308
   }; // Redirections

   enum ClientErrors {
      ClientError                 = 400,

      BadRequest                  = 400,
      Unauthorized                = 401,
      PaymentRequired             = 402,
      Forbidden                   = 403,
      NotFound                    = 404,
      MethodNotAllowed            = 405,
      NotAcceptable               = 406,
      ProxyAuthenticationRequired = 407,
      RequestTimeout              = 408,
      Conflict                    = 409,
      Gone                        = 410,
      LengthRequired              = 411,
      PreconditionFailed          = 412,
      PayloadTooLarge             = 413,
      URITooLong                  = 414,
      UnsupportedMediaType        = 415,
      RangeNotSatisifiable        = 416,
      ExpectationFailed           = 417,
      ImATeapot                   = 418,  // RFC 2324

      MisdirectedRequest          = 421,
      UnprocessableEntity         = 422,
      Locked                      = 423,
      FailedDependency            = 424,

      UpgradeRequired             = 426,

      PreconditionRequired        = 428,
      TooManyRequests             = 429,

      RequestHeaderFieldsTooLarge = 431,

      UnavailableForLegalReasons  = 451  // Fahrenheit 451
   }; // ClientErrors

   enum ServerErrors {
      ServerError                   = 500,

      InternalServerError           = 500,
      NotImplemented                = 501,
      BadGateway                    = 502,
      ServiceUnavailable            = 503,
      GatewayTimeout                = 504,
      HTTPVersionNotSupported       = 505,
      VariantAlsoNegotiates         = 506,
      InsufficientStorage           = 507,
      LoopDetected                  = 508,

      NotExtended                   = 510,
      NetworkAuthenticationRequired = 511
   }; // ServerErrors

public: // Methods

   explicit Response(String response);

public: // Variables

   const String version;

   const String code;

   const String reason;

   const MapSet headers;

}; // Response

#endif // Response_hh
