#include "TrafficData.h"
#include "Utils.h"

#include <iomanip>

namespace collector
{

bool Socket::operator<(const Socket& r) const
{
    if(ip != r.ip)
        return ip < r.ip;

    return port < r.port;
}

std::ostream& operator<<(std::ostream& out, const Socket& s)
{
    return out << utils::ip_to_str(s.ip) << ":" << s.port;
}

std::ostream& operator<<(std::ostream& out, const ProtocolData& s)
{
    return out << s.src << " -> " << s.dst;
}

std::ostream& operator<<(std::ostream& out, const SocketTraffic& s)
{
    for(const auto& [s, c] : s.per_socket_count)
    {
        out << "    * " << s << ", count: " << c << std::endl;
    }

    return out << "    Total: " << s.total_count;
}

PortTraffic& PortTraffic::operator+=(const ProtocolData& pt)
{
    SocketTraffic& elem = amount_per_dest_port[pt.dst.port];
    ++elem.per_socket_count[pt.src];
    ++elem.total_count;
    ++total_count;

    return *this;
}

std::ostream& operator<<(std::ostream& out, const PortTraffic& s)
{
    auto loct{std::localtime(&s.observation_time)};
    out << "Time: " << std::put_time(loct, "%c") << ". Total port access count: " << s.total_count << std::endl;
    for(const auto& [p, s] : s.amount_per_dest_port)
    {
        out << "  Port: " << p << " accessed by:" << std::endl;
        out << s << std::endl;
    }

    return out;
}

}
