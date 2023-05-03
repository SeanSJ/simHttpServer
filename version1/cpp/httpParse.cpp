#include"../include/httpparse.h"
#include"../include/utils.h"

#include<string>
#include<iostream>
#include<algorithm>



std:: unordered_map<std::string, http:: HttpRequest:: HTTP_HEADER> http:: HttpRequest:: header_map = {

		{"HOST", http::HttpRequest:: Host},
		{"USER-AGENT", http::HttpRequest::User_Agent},
		{"CONNECTION", http:: HttpRequest::Connection},
		{"ACCEPT-ENCODING", http::HttpRequest::Accept_Encoding},
		{"ACCEPT-LANGUAGE", http::HttpRequest::Accept_Language},
		{"ACCEPT", http::HttpRequest::Accept},
		{"CACHE-CONTROL", http::HttpRequest::Cache_Control},
		{"UPGRADE-INSECURE-REQUESTS", http::HttpRequest::Upgrade_Insecure_Requests}

};


// 解析初始数据

http::HttpRequestParser::LINE_STATE  http::HttpRequestParser::parse_line(char *buffer, int &checked_index, int &read_index){

		char tmp;
		
		for(; checked_index < read_index; checked_index++){

				tmp = buffer[checked_index];
				if(tmp == CR){
						// CR '\r'
						if(checked_index + 1 == read_index)
								return LINE_MORE;

						// LF  \r\n
						if(buffer[checked_index+1] == LF){
								buffer[checked_index++] = LINE_END;
								buffer[checked_index++] = LINE_END;
								return LINE_OK;
						}

						return LINE_BAD;
				}
		}// for

		return LINE_MORE;
}

// 解析请求行
http::HttpRequestParse :: HTTP_CODE http::HttpRequestParser::parse_requestline(char *line, PARSE_STATE &parse_state, HttpRequest& request){

		char *url = strpbrk(line, " \t");
		if(!url){
				return BAD_REQUEST;
		}

		// 
		*url++ = '\0';

		char *method = line;

		if(strcasecmp(method, "GET") == 0){
				request.mMethod = HttpRequest:: GET;
		} else if(strcasecmp(method, "POST") == 0){
				request.mMethod = HttpRequest::POST;
		} else if(strcasecmp(method, "PUT")== 0) {
				request.mMethod = HttpRequest::PUT;
		} else{
			   return BAD_REQUEST;
		}


		url += strspn(url, " \t");

		char *version = strpbrk(url, " \t");
		if(!version){
				return BAD_REQUEST;
		}
		*version++ = '\0';
		version += strspn(version, " \t");

		
		if(strncasecmp("HTTP/1.1", version, 8) == 0){
				request.mVersion = HttpRequest::HTTP_11;
		} else if(strncasecmp("HTTP/1.0", version, 8) == 0){
				request.mVersion = HttpRequest::HTTP_10;
		} else{
				return BAD_REQUEST;
		}

		if(strncasecmp(url, "http://", 7) == 0){
				url += 7;
				url = strchr(url , '/');
		} else if(strncasetmp(url, "/", 1) == 0){
				PASS;
		} else{
				return BAD_REQUEST;
		}

		
		if(!url || *url != '/'){
				return BAD_REQUEST;
		}

		request.mUrl = std::string(url);

		parse_state = PARSE_STATE;
		return NO_REQUEST;
}		
		


http:: HttpRequestParser::HTTP_CODE http::HttpRequestParser::parse_headers(char *line, PARSE_STATE &parse_state, HttpRequest &request){

		if(*line == '\0'){
				if(request.mMethod == HttpRequest::GET){
						return NO_REQUEST;
				}
				parse_state = PARSE_BODY;
				return NO_REQUEST;
		}

		char key[100], value[100];

		sscanf(line, "%[^:]:%[^:]", key, value);

		decltype(HttpRequest::header_map) :: iterator it;
		std::string key_s(key);
		std::transform(key_s.begin(), key_s.end(), key_s.begin(), ::toupper);
		str::string value_s(value);
		if(key_s == std::string("UPGRADE-INSECURE-REQUESTS")){
				return NO_REQUEST;
		}

		if( (it == HttpRequest::header_map.find(util::trim(key_s))) != (HttpRequest::header_map.end())){
				request.mHeaders.insert(std::make_pair(it->second, util::trim(value_s)));
		} else{
				std::cout<< "Header no suppoort "<< key<< " : "<<value<,std::endl;
		}

		return NO_REQUEST;
}


http::HttpRequestParser:: HTTP_CODE http::HttpRequestParser::parse_body(char *body, http::HttpRequest & request){
		request.mContent = body;
		return GET_REQUEST;
}


http::HttpRequestParser::HTTP_CODE http::HttpRequestParser:: parse_content(char &buffer, int &check_index, int &read_index, 
				http::HttpRequestParser::PARSE_STATE &parse_state, int &start_line,
				HttpRequest & request){
		
		LINE_STATE line_state = LINE_OK;
		HTTP_CODE retcode = NO_REQUEST;
		while( ( line_state = parse_line(buffer, check_index, read_index)) == LINE_OK) {
				char * tmp = buffer + start_line;
				start_line = check_index;

				switch(parse_state){
						case PARSE_REQUESTLINE: {
														retcode = parse_requestline(tmp, parse_state, request);
														if(retcode == BAD_REQUEST)
																return BAD_REQUEST;
														break;
												}
						case PARSE_HEADER: {
												   retcode = parse_headers(tmp , parse_state, request);
												   if(retcode == BAD_REQUEST)
														   return BAD_REQUEST;
												   else if(retcode == GET_REQUEST)
														   return GET_REQUEST;
												   break;
										 }

						case PARSE_BODY: {
														retcode = parse_body(tmp, request);
														if(retcode == GET_REQUEST)
																return GET_REQUEST;
														return BAD_REQUEST;
										}
						default:
										 return INTERNAL_ERROR;
				}
		} // while 


		if(line_state == LINE_MORE){
				return NO_REQUEST;
		} else {
				return BAD_REQUEST;
		}
}



std::ostream & http::operator<<(std::ostream &os, const http::HttpRequest&request){
		os<<"method:"<<request.mMethod<<std::endl;
		os<<"url:"<<request.mUrl<<std::endl;
		os<<"version:"<<request.mVersion<<std::endl;
		
		for(auto it = request.mHeaders.begin(); it != request.mHeaders.end(); it++)
				os<<it->first<< " : "<<it->second<<std::endl;
		return os;
}










