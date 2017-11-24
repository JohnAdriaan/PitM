//
// Response.hh
//

#ifndef Response_hh
#define Response_hh

#include "HTTP.hh"

class WWW::HTTP::Response {

public: // Enums

   enum Categories {
      Informational = 100,
      Success       = 200,
      Redirection   = 300,
      ClientError   = 400,
      ServerError   = 500
   }; // Categories

   enum Codes {
      Continue           = 100,
      SwitchingProtocols = 101,
      Processing         = 102,


      OK                = 200,
      Created           = 201,
      Accepted          = 202,
      NonAuthoritative  = 203,
      NoContent         = 204,
      ResetContent      = 205,
      PartialContent    = 206,
      MultiStatus       = 207,
      AlreadyReported   = 208,
      IMUsed            = 226,


      MultipleChoices   = 300,
      MovedPermanently  = 301,
      Found             = 302,
      SeeOther          = 303,
      NotModifed        = 304,
      UseProxy          = 305,
      SwitchProxy       = 306,
      TemporaryRedirect = 307,
      PermanentRedirect = 308,


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

      UnavailableForLegalReasons  = 451, // Fahrenheit 451


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
   }; // Codes

public: // Static methods

   static Categories Category(Codes code);

public: // Methods

   // Use this version if you know the body length, but don't have the String.
   // A ContentLength will be added with that length
   Response(Versions version, Codes code, Size length);

   Response(Versions version, Codes code, const String &body=String());

   void Add(const String &key, Set &values);

   // If key is already there, add value as part of its Set
   void Add(const String &key, int value);
   void Add(const String &key, unsigned value);
   void Add(const String &key, const String &value);
   void Add(const String &key, const String &field, int value);
   void Add(const String &key, const String &field, unsigned value);

   // Append to body
   void Append(const String &lines);

   operator String() const;

private: // Variables

   String response;

   MapSet headers;

   String body;

}; // Response

#endif // Response_hh
