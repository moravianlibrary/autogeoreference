/* 
 * File:   main.cpp
 * Author: dudae
 *
 * Created on Štvrtok, 2015, júla 23, 10:41
 */

#include <fcgio.h>
#include <gdal.h>

#include <Poco/Exception.h>

#include "Controller.h"
#include "RequestException.h"
#include "appio.h"

using namespace std;

int main( int argc, char** argv ) {
    FCGX_Request request;
    
    FCGX_Init();
    FCGX_InitRequest(&request, 0, 0);
    
    GDALAllRegister();
    
    while (FCGX_Accept_r(&request) == 0) {
        fcgi_streambuf cin_fcgi_streambuf(request.in);
        fcgi_streambuf cout_fcgi_streambuf(request.out);
        fcgi_streambuf cerr_fcgi_streambuf(request.err);
        
        cin.rdbuf(&cin_fcgi_streambuf);
        cout.rdbuf(&cout_fcgi_streambuf);
        cerr.rdbuf(&cerr_fcgi_streambuf);
        
        try {
            string action = appio::get_request_action(request);
            
            if (action == "autogeoreference") {
                Controller::autoGeoreference(appio::get_request_params(request));
            } else {
                throw RequestException("Invalid url.");
            }
        } catch (Poco::Exception& e) {
            appio::print_error(e.displayText());
        } catch (exception& e) {
            appio::print_error(e.what());
        }
    }
    
    return 0;
}