// Copyright 2007 Google Inc. All Rights Reserved.
// Author: chaiying@google.com

#ifndef WEBSITE_TOOLS_SITEMAP_SITEMAPSERVICE_HTTP_H__
#define WEBSITE_TOOLS_SITEMAP_SITEMAPSERVICE_HTTP_H__

#include <string>
#include <map>
#include <vector>

#include "sitemapservice/activesocket.h"

class HttpProto {
public:
  // The type of the callback function for sending the HTTP response content
  typedef   void (*response_func) (HttpProto*); 

  // Parses the HTTP params from the 'paramStr', store the results in params_
  void ParseParams(std::string paramStr);

  // The constructor
  HttpProto();

  // Reset the HTTP request fields to default value
  void ResetRequest();

  // Reset the HTTP response fields to default value
  void ResetResponse();

  // Processes the HTTP request, returns true if the parsing of the HTTP
  // request is successful.
  bool ProcessRequest(const ActiveSocket& s);

  // Processes the HTTP response, returns true if the parsing of the HTTP 
  // response is successful.
  bool ProcessResponse(const ActiveSocket& s);

  // Returns the param value in the params_ according to the 'paramName'
  std::string GetParam(const std::string& paramName);

  // Returns true if the last modified time of the client cache equals to the
  // 'lastWrite'. Sets the answer_last_modified_ according to the 'lastWrite' 
  // and sets the answer_cache_control_ to one day.
  bool CheckCaching(const time_t& lastWrite);

private:

  // Returns the string that in the 'str' and begin with 'begin', end with 'end'
  static std::string FindSubString(const std::string& str, 
    const std::string& begin, 
    const std::string& end);



  // replace '+' to ' ' in the param value
  static void UnescapeWhitespace(std::string* val);

  // Parse the first line of the HTTP request
  bool ParseFirstLine(const ActiveSocket& s);

public:

  // The names of the predefined HTTP parameters for the Setting Editor
  static const std::string kSessionIdParamName; // Session id param
  static const std::string kUsernameParamName;  // Username param
  static const std::string kPasswordParamName;  // Password param

  // The first line of the HTTP request
  std::string method_; // "GET" or "POST"
  std::string url_;  // The URL of the request
  std::string path_; // The URI of the request

  // The parameters in URL, POST content, and 'referer' header
  std::map<std::string, std::string> params_;

  // The headers of the HTTP request
  std::string accept_; // The 'Accept' header
  std::string accept_language_;// The 'accept-language' header
  std::string accept_encoding_;// The 'accept-encoding' header

  std::string if_none_match_;// The 'if-none-match' header 
  std::string if_modified_since_;// The 'if-modified-since' header 

  std::string user_agent_;// The 'user-agent' header   
  std::string referer_;// The referrer header   
  std::string cookie_;// The cookie   

  std::string content_type_;// The content type 
  int content_length_;// The content length   

  int range_first_;// The MIN value in the 'range' header
  int range_last_;// The MAX value in the 'range' header  

  // The headers of the HTTP response  
  std::string answer_status_;// The status line, such as '202 OK'
  std::string answer_location_;// The location   
  std::string answer_cache_control_;// The cache control header   
  std::string answer_last_modified_;// The last modified header 

  std::string answer_content_type_;// The content type  
  std::string answer_content_range_;// The content length    
  int answer_content_length_;// The content length 


  // The callback function for sending the HTTP response content
  response_func answer_callback_;

  // The the HTTP response content, if answer_callback_ is not null, 
  // answer_ will be ignored.
  std::string answer_;

  // The client ip
  // If x_forwarded_for is not empty in http header, rempte_ip_ is set
  // to x_forwarded_for. Otherwise, it's the value read from socket.
  std::string remote_ip_;
};
#endif