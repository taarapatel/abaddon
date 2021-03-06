#include "http.hpp"

//#define USE_LOCAL_PROXY
HTTPClient::HTTPClient(std::string api_base)
    : m_api_base(api_base) {
    m_dispatcher.connect(sigc::mem_fun(*this, &HTTPClient::RunCallbacks));
}

void HTTPClient::SetUserAgent(std::string agent) {
    m_agent = agent;
}

void HTTPClient::SetAuth(std::string auth) {
    m_authorization = auth;
}

void HTTPClient::MakeDELETE(std::string path, std::function<void(cpr::Response r)> cb) {
    printf("DELETE %s\n", path.c_str());
    auto url = cpr::Url { m_api_base + path };
    auto headers = cpr::Header {
        { "Authorization", m_authorization },
    };
    auto ua = cpr::UserAgent {m_agent != "" ? m_agent : "Abaddon" };

#ifdef USE_LOCAL_PROXY
    m_futures.push_back(cpr::DeleteCallback(
        std::bind(&HTTPClient::OnResponse, this, std::placeholders::_1, cb),
        url, headers, ua,
        cpr::Proxies { { "http", "127.0.0.1:8888" }, { "https", "127.0.0.1:8888" } },
        cpr::VerifySsl { false }));
#else
    m_futures.push_back(cpr::DeleteCallback(
        std::bind(&HTTPClient::OnResponse, this, std::placeholders::_1, cb),
        url, headers, ua));
#endif
}

void HTTPClient::MakePATCH(std::string path, std::string payload, std::function<void(cpr::Response r)> cb) {
    printf("PATCH %s\n", path.c_str());
    auto url = cpr::Url { m_api_base + path };
    auto headers = cpr::Header {
        { "Authorization", m_authorization },
        { "Content-Type", "application/json" },
    };
    auto ua = cpr::UserAgent { m_agent != "" ? m_agent : "Abaddon" };

    auto body = cpr::Body { payload };
#ifdef USE_LOCAL_PROXY
    m_futures.push_back(cpr::PatchCallback(
        std::bind(&HTTPClient::OnResponse, this, std::placeholders::_1, cb),
        url, headers, body, ua,
        cpr::Proxies { { "http", "127.0.0.1:8888" }, { "https", "127.0.0.1:8888" } },
        cpr::VerifySsl { false }));
#else
    m_futures.push_back(cpr::PatchCallback(
        std::bind(&HTTPClient::OnResponse, this, std::placeholders::_1, cb),
        url, headers, body, ua));
#endif
}

void HTTPClient::MakePOST(std::string path, std::string payload, std::function<void(cpr::Response r)> cb) {
    printf("POST %s\n", path.c_str());
    auto url = cpr::Url { m_api_base + path };
    auto headers = cpr::Header {
        { "Authorization", m_authorization },
        { "Content-Type", "application/json" },
    };
    auto ua = cpr::UserAgent { m_agent != "" ? m_agent : "Abaddon" };

    auto body = cpr::Body { payload };
#ifdef USE_LOCAL_PROXY
    m_futures.push_back(cpr::PostCallback(
        std::bind(&HTTPClient::OnResponse, this, std::placeholders::_1, cb),
        url, headers, body, ua,
        cpr::Proxies { { "http", "127.0.0.1:8888" }, { "https", "127.0.0.1:8888" } },
        cpr::VerifySsl { false }));
#else
    m_futures.push_back(cpr::PostCallback(
        std::bind(&HTTPClient::OnResponse, this, std::placeholders::_1, cb),
        url, headers, body, ua));
#endif
}

void HTTPClient::MakePUT(std::string path, std::string payload, std::function<void(cpr::Response r)> cb) {
    printf("PUT %s\n", path.c_str());
    auto url = cpr::Url { m_api_base + path };
    auto headers = cpr::Header {
        { "Authorization", m_authorization },
        { "Content-Type", "application/json" },
    };
    auto ua = cpr::UserAgent { m_agent != "" ? m_agent : "Abaddon" };

    auto body = cpr::Body { payload };
#ifdef USE_LOCAL_PROXY
    m_futures.push_back(cpr::PutCallback(
        std::bind(&HTTPClient::OnResponse, this, std::placeholders::_1, cb),
        url, headers, body, ua,
        cpr::Proxies { { "http", "127.0.0.1:8888" }, { "https", "127.0.0.1:8888" } },
        cpr::VerifySsl { false }));
#else
    m_futures.push_back(cpr::PutCallback(
        std::bind(&HTTPClient::OnResponse, this, std::placeholders::_1, cb),
        url, headers, body, ua));
#endif
}

void HTTPClient::MakeGET(std::string path, std::function<void(cpr::Response r)> cb) {
    printf("GET %s\n", path.c_str());
    auto url = cpr::Url { m_api_base + path };
    auto headers = cpr::Header {
        { "Authorization", m_authorization },
        { "Content-Type", "application/json" },
    };
    auto ua = cpr::UserAgent { m_agent != "" ? m_agent : "Abaddon" };

    auto x = cpr::UserAgent {};
#ifdef USE_LOCAL_PROXY
    m_futures.push_back(cpr::GetCallback(
        std::bind(&HTTPClient::OnResponse, this, std::placeholders::_1, cb),
        url, headers, ua,
        cpr::Proxies { { "http", "127.0.0.1:8888" }, { "https", "127.0.0.1:8888" } },
        cpr::VerifySsl { false }));
#else
    m_futures.push_back(cpr::GetCallback(
        std::bind(&HTTPClient::OnResponse, this, std::placeholders::_1, cb),
        url, headers, ua));
#endif
}

void HTTPClient::CleanupFutures() {
    for (auto it = m_futures.begin(); it != m_futures.end();) {
        if (it->wait_for(std::chrono::seconds(0)) == std::future_status::ready)
            it = m_futures.erase(it);
        else
            it++;
    }
}

void HTTPClient::RunCallbacks() {
    m_mutex.lock();
    m_queue.front()();
    m_queue.pop();
    m_mutex.unlock();
}

void HTTPClient::OnResponse(cpr::Response r, std::function<void(cpr::Response r)> cb) {
    CleanupFutures();
    try {
        m_mutex.lock();
        m_queue.push([this, r, cb] { cb(r); });
        m_dispatcher.emit();
        m_mutex.unlock();
    } catch (std::exception &e) {
        fprintf(stderr, "error handling response (%s, code %d): %s\n", r.url.c_str(), r.status_code, e.what());
    }
}
