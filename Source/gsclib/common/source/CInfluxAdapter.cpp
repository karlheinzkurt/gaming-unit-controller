
#include "../include/CInfluxAdapter.h"

#include <cpprest/http_client.h>

#include <stdexcept>

namespace GSC { namespace Common {
   
struct CInfluxAdapter::Impl   
{
   Impl(web::uri uri, std::string db) 
      :m_client(uri) 
      ,m_queryURI(web::uri_builder("/query").append_query("db", db).to_uri())
      ,m_writeURI(web::uri_builder("/write").append_query("db", db).to_uri())
      ,m_pingURI(web::uri_builder("/ping").to_uri())
   {
      createDatabase(db).then([](web::http::http_response response)
      {
         if (response.status_code() != web::http::status_codes::OK)
         {  throw std::runtime_error("Could not create database"); }
      }).wait();
   }
   
   pplx::task<web::http::http_response> createDatabase(std::string name)
   {
      web::http::http_request request(web::http::methods::POST);
      std::ostringstream os;
      os << "CREATE DATABASE \"" << name << "\"";
      request.set_request_uri(web::uri_builder("/query").append_query("q", os.str()).to_uri());
      return m_client.request(request);
   }
   
   pplx::task<web::http::http_response> write(std::string data)
   {
      web::http::http_request request(web::http::methods::POST);
      request.set_request_uri(m_writeURI);
      request.set_body(utility::string_t(data));
      return m_client.request(request);
   }
   
   pplx::task<web::http::http_response> ping()
   {  return m_client.request(web::http::methods::GET, m_pingURI.to_string()); }
   
   web::http::client::http_client m_client;
   web::uri m_queryURI;
   web::uri m_writeURI;
   web::uri m_pingURI;
};
   
CInfluxAdapter::CInfluxAdapter() : m_impl(std::make_unique<Impl>(web::uri_builder("http://localhost:8086").to_uri(), "gsc"))
{
    /**  \todo Create empty series for 'active' and 'exceeding'
    */
}

CInfluxAdapter::~CInfluxAdapter() = default;

void CInfluxAdapter::insertActive(API::IMatch::SetType const& active)
{
   if (active.empty()) return;
   
   std::ostringstream os;
   os << "active system=1.0 ";
   for (auto const& a : active) { os << a->getName() << "=1.0 "; }
   m_impl->write(os.str()).wait();
}

void CInfluxAdapter::insertExceeding(std::set<std::string> const& exceeding)
{
   if (exceeding.empty()) return;
   
   std::ostringstream os;
   os << "exceeding ";
   for (auto const& e : exceeding) { os << e << "=1.0 "; }
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
