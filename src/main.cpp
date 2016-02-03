#include <iostream>
#include <boost/asio.hpp>

int main() {
    boost::asio::io_service service;
    boost::asio::ip::tcp::socket sock(service);
    //boost::asio::ip::tcp::endpoint ep(boost::asio::ip::address::from_string(), 8000);
    //std::cout << ep.address().to_string() << ":" << ep.port() << "/" << ep.protocol().protocol() << std::endl;
    boost::asio::ip::tcp::resolver resolver(service);
    boost::asio::ip::tcp::resolver::query query(boost::asio::ip::tcp::v4(), "10.20.16.86", "8000");
    boost::asio::ip::tcp::resolver::iterator iterator = resolver.resolve(query);

    boost::system::error_code ec;
    boost::asio::connect(sock, iterator, ec);
    if (ec) {
        std::cout << "connect failure: " << ec.message() << std::endl;
    }

    boost::asio::streambuf sbw;
    std::ostream os(&sbw);
    os << "GET / HTTP/1.1\r\n"
       << "Host: 10.20.16.83:8040\r\n"
       << "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8\r\n"
       << "User-Agent: Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/47.0.2526.80 Safari/537.36\r\n"
       << "\r\n";
    boost::asio::write(sock, sbw, boost::asio::transfer_exactly(sbw.size()));
    //char buff[1024] = {0};
    boost::asio::streambuf sbr;
    //sock.read_some(boost::asio::buffer(buff,1024));
    boost::asio::read(sock, sbr, boost::asio::transfer_exactly(sbr.size()));
    boost::asio::streambuf::const_buffers_type bufs = sbr.data();
    //std::string line(bufs);
    std::copy(boost::asio::buffers_begin(bufs), boost::asio::buffers_begin(bufs)+sbr.size(), std::ostream_iterator<char>(std::cout, " "));
    sock.shutdown(boost::asio::ip::tcp::socket::shutdown_receive);
    sock.close();
}