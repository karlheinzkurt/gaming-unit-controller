
#include "../include/CInfluxAdapter.h"

#include <cpprest/http_client.h>

#include <log4cxx/logger.h>

#include <stdexcept>

namespace GSC { namespace Common {
   
struct CInfluxAdapter::Impl   
{
   static web::http::client::http_client_config doConfig()
   {
      web::http::client::http_client_config config;
      config.set_timeout(config.timeout()); ///< Keep default but maybe we should decrease it to a few s
      return config;
   }
   
   Impl(web::uri uri, std::string db) 
      :m_config(doConfig())
      ,m_client(uri, m_config) 
      ,m_queryURI(web::uri_builder("/query").append_query("db", db).to_uri())
      ,m_writeURI(web::uri_builder("/write").append_query("db", db).to_uri())
      ,m_pingURI(web::uri_builder("/ping").to_uri())
      ,m_logger(log4cxx::Logger::getLogger("GSC.Common.CInfluxAdapter"))
   {
      createDatabase(db).wait();
      LOG4CXX_INFO(m_logger, "Created database: " << db);
   }
   
   pplx::task<web::http::http_response> createDatabase(std::string name)
   {
      web::http::http_request request(web::http::methods::POST);
      std::ostringstream os;
      os << "CREATE DATABASE \"" << name << "\"";
      request.set_request_uri(web::uri_builder("/query").append_query("q", os.str()).to_uri());
      return doRequest(std::move(request), web::http::status_codes::OK);
   }
   
   pplx::task<web::http::http_response> write(std::string data)
   {
      web::http::http_request request(web::http::methods::POST);
      request.set_request_uri(m_writeURI);
      request.set_body(utility::string_t(data));
      return doRequest(std::move(request), web::http::status_codes::NoContent);
   }
   
   pplx::task<web::http::http_response> ping()
   {  
      web::http::http_request request(web::http::methods::GET);
      request.set_request_uri(m_pingURI);
      return doRequest(std::move(request), web::http::status_codes::NoContent);
   }
   
   pplx::task<web::http::http_response> doRequest(web::http::http_request&& request, web::http::status_code expect)
   {  
      LOG4CXX_INFO(m_logger, "HTTP request: " << request.to_string());
      return m_client.request(std::move(request)).then([this, expect](web::http::http_response response)
      {
         if (response.status_code() != expect)
         {  
            LOG4CXX_INFO(m_logger, "Request unsuccessful with code: " << response.status_code());
            throw std::runtime_error("Request unsuccessful"); 
         }
         return response;
      });
   }
   
private:
   web::http::client::http_client_config m_config;
   web::http::client::http_client m_client;
   web::uri m_queryURI;
   web::uri m_writeURI;
   web::uri m_pingURI;
   log4cxx::LoggerPtr m_logger;
};
   
CInfluxAdapter::CInfluxAdapter() : m_impl(std::make_unique<Impl>(web::uri_builder("http://localhost:8086").to_uri(), "gsc"))
{
    /**  \todo Create empty series for 'active' and 'exceeding'
    */
}

CInfluxAdapter::~CInfluxAdapter() = default;

void CInfluxAdapter::insertActive(API::IMatch::SetType const& active)
{  
   std::ostringstream os;
   os << "active system=1.0\n";
   for (auto const& a : active) { os << "active " << a->getName() << "=1.0\n"; }
   m_impl->write(os.str()).wait();
}

void CInfluxAdapter::insertExceeding(API::IMatch::SetType const& exceeding)
{
   if (exceeding.empty()) return;
   
   std::ostringstream os;
   for (auto const& e : exceeding) { os << "exceeding " << e->getName() << "=1.0\n"; }
   m_impl->write(os.str()).wait();
}

void CInfluxAdapter::ping() 
{  m_impl->ping().wait(); }

   /*std::random_device rd;
   std::mt19937 mt(rd());
   std::uniform_real_distribution<double> dist(0., 10.);
   std::mutex mutex;
   pplx::cancellation_token_source cancelationOption;
   
   std::queue<pplx::task<web::http::http_response>> tasks;
   
   for (unsigned int i{0}; i < 10000; ++i)
   {
      std::ostringstream os;
      os << "minecraft,active=true value=" << dist(mt);
      utility::string_t const message(os.str());
      web::http::http_request request(web::http::methods::POST);
      request.set_request_uri(web::uri_builder("/write").append_query("db", "gsc")).to_string());
      request.set_body(message);
      
      tasks.emplace(m_impl->m_client.request(request, cancelationOption.get_token()).then([i, &mutex](auto result)
      {
         //std::unique_lock<std::mutex> lock(mutex);
         std::cout << i << '\n';
         return result;
      }));
   }
   while (!tasks.empty())
   {
      try 
      {
         tasks.front().wait();
         tasks.pop();
      }
      catch (std::exception const& e)
      {  
         tasks.pop();
         std::cerr << e.what() << '\n'; 
      }
   }*/
   
   /*
   .then([](web::http::http_response response)
   {
      if (response.status_code() != web::http::status_codes::OK)
      {
        std::cerr << response.status_code();
      }
   })
   .wait();
   */
}}
