#include "AuthNService.h"
#include <algorithm>

AuthNService::AuthNService()
{
}

AuthNService::~AuthNService()
{
}

bool AuthNService::verify(std::string accessToken)
{
  std::lock_guard<std::mutex> lock(m_tokensMutex);
  return std::find(m_tokens.begin(), m_tokens.end(), accessToken) != m_tokens.end();
}

grpc::Status AuthNService::Login(grpc::ServerContext* context, const auth::Credentials* request, auth::AccessToken* response)
{
  std::cout << "Try login credentials: " << request->username() << " " << request->password() << std::endl;
  // create a random generated 6 digit token as a string
  std::string dummyToken = std::string("randSixDigitToken_") + std::to_string(rand() % 999999 + 100000);
  {
    std::lock_guard<std::mutex> lock(m_tokensMutex);
    m_tokens.push_back(dummyToken);
  }
  response->set_token(dummyToken);
  return grpc::Status::OK;
}
