#pragma once
#include "torrent_messages.h"
#include "client_errors.h"

#include <bencoding/be_torrent_file_parse.h>
#include <bencoding/be_tracker_response_parse.h>
#include <small_utils/utils_bytes.h>

#include <asio.hpp>

namespace be
{
    struct TorrentPeer
    {
        explicit TorrentPeer(asio::io_context& io_context);

        // Connect & Handshake.
        asio::awaitable<outcome::result<void>> start(
            const PeerAddress& address
            , const SHA1Bytes& info_hash
            , const PeerId& peer_id);

        asio::io_context* io_context_ = nullptr;
        asio::ip::tcp::socket socket_;
        // Information about peer that we are connected to.
        PeerId peer_id_;
        ExtensionsBuffer extensions_;
        Message_Bitfield bitfield_;
        bool unchocked_ = false;
    };

    struct TorrentClient
    {
        static outcome::result<TorrentClient> make(
            const char* torrent_file_path
            , std::random_device& random);

        struct HTTPTrackerRequest
        {
            std::string host_;
            std::uint16_t port_ = 0;
            std::string get_uri_;
            bool use_https_ = false;
        };

        struct RequestInfo
        {
            std::uint16_t server_port = 0;
            std::uint32_t pieces_count = 0;
            std::uint32_t uploaded_pieces = 0;
            std::uint32_t downloaded_pieces = 0;
        };

        outcome::result<HTTPTrackerRequest> get_tracker_request_info(
            const RequestInfo& request) const;

        asio::awaitable<outcome::result<be::TrackerResponse>>
            request_torrent_peers(asio::io_context& io_context, const RequestInfo& request);

        std::uint32_t get_pieces_count() const;
        std::uint64_t get_total_size_bytes() const;
        std::uint32_t get_piece_size_bytes() const;

    public:
        TorrentMetainfo metainfo_;
        SHA1Bytes info_hash_;
        PeerId peer_id_;
    };
} // namespace be