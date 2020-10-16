#pragma once
#include <bencoding/be_torrent_file_parse.h>
#include <small_utils/utils_bytes.h>

#include <optional>

namespace be
{
    struct TorrentClient
    {
        static std::optional<TorrentClient> make(
            const char* torrent_file_path
            , std::random_device& random);

        struct HTTPTrackerRequest
        {
            std::string host_;
            std::uint16_t port_ = 0;
            std::string get_uri_;
        };

        std::optional<HTTPTrackerRequest> get_tracker_request_info(
            std::uint16_t server_port = 6882) const;

    public:
        TorrentMetainfo metainfo_;
        SHA1Bytes info_hash_;
        PeerId peer_id_;
    };
} // namespace be