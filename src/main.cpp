#include <iostream>
#include <boost/asio.hpp>

char peer0_0[] = {
    0x0c, 0x01, 0x18, 0x7b, 0x00, 0x01, 0x20, 0x00, 
    0x20, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00 };

int main() {
    boost::asio::io_service service;
    boost::asio::ip::tcp::socket sock(service);
    //boost::asio::ip::tcp::endpoint ep(boost::asio::ip::address::from_string(), 8000);
    //std::cout << ep.address().to_string() << ":" << ep.port() << "/" << ep.protocol().protocol() << std::endl;
    boost::asio::ip::tcp::resolver resolver(service);
    boost::asio::ip::tcp::resolver::query query(boost::asio::ip::tcp::v4(), "222.73.49.4", "7709");
    boost::asio::ip::tcp::resolver::iterator iterator = resolver.resolve(query);

    boost::system::error_code ec;
    boost::asio::connect(sock, iterator, ec);
    if (ec) {
        std::cout << "connect failure: " << ec.message() << std::endl;
        return 1;
    }

    //boost::asio::streambuf sbw;
    //std::ostream os(&sbw);
    //os << "GET / HTTP/1.1\r\n"
    //   << "Host: 10.20.16.83:8040\r\n"
    //   << "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8\r\n"
    //   << "User-Agent: Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/47.0.2526.80 Safari/537.36\r\n"
    //   << "\r\n";
    boost::asio::write(sock, boost::asio::buffer(peer0_0), boost::asio::transfer_exactly(sizeof(peer0_0)));
    //char buff[1024] = {0};
    boost::asio::streambuf sbr;
    boost::system::error_code err;
    //sock.read_some(boost::asio::buffer(buff,1024));
    boost::asio::read(sock, sbr, boost::asio::transfer_exactly(70), err);
    if (err) {
        std::cout << err.message() << std::endl;
        return 2;
    }
    boost::asio::streambuf::const_buffers_type bufs = sbr.data();
    for (int i=0; i<70; ++i) {
        std::cout << std::hex << int(*boost::asio::buffers_begin(bufs));
    }
    std::cout << std::endl;
    std::cout << sbr.size() << std::endl;
    //std::copy(boost::asio::buffers_begin(bufs), boost::asio::buffers_begin(bufs)+sbr.size(), std::ostream_iterator<char>(std::cout, ""));
    sock.shutdown(boost::asio::ip::tcp::socket::shutdown_receive);
    sock.close();
}