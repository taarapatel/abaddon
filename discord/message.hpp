#pragma once
#include "snowflake.hpp"
#include "json.hpp"
#include "user.hpp"
#include "sticker.hpp"
#include "emoji.hpp"
#include <string>
#include <vector>

enum class MessageType {
    DEFAULT = 0,
    RECIPIENT_ADD = 1,
    RECIPIENT_REMOVE = 2,
    CALL = 3,
    CHANNEL_NAME_CHANGE = 4,
    CHANNEL_ICON_CHANGE = 5,
    CHANNEL_PINNED_MESSAGE = 6,
    GUILD_MEMBER_JOIN = 7,
    USER_PREMIUM_GUILD_SUBSCRIPTION = 8,
    USER_PREMIUM_GUILD_SUBSCRIPTION_TIER_1 = 9,
    USER_PREMIUM_GUILD_SUBSCRIPTION_TIER_2 = 10,
    USER_PREMIUM_GUILD_SUBSCRIPTION_TIER_3 = 11,
    CHANNEL_FOLLOW_ADD = 12,
    GUILD_DISCOVERY_DISQUALIFIED = 14,
    GUILD_DISCOVERY_REQUALIFIED = 15,
    INLINE_REPLY = 19,
};

enum class MessageFlags {
    NONE = 0,
    CROSSPOSTED = 1 << 0,
    IS_CROSSPOST = 1 << 1,
    SUPPRESS_EMBEDS = 1 << 2,
    SOURCE_MESSAGE_DELETE = 1 << 3,
    URGENT = 1 << 4,
};

struct EmbedFooterData {
    std::string Text;
    std::optional<std::string> IconURL;
    std::optional<std::string> ProxyIconURL;

    friend void to_json(nlohmann::json &j, const EmbedFooterData &m);
    friend void from_json(const nlohmann::json &j, EmbedFooterData &m);
};

struct EmbedImageData {
    std::optional<std::string> URL;
    std::optional<std::string> ProxyURL;
    std::optional<int> Height;
    std::optional<int> Width;

    friend void to_json(nlohmann::json &j, const EmbedImageData &m);
    friend void from_json(const nlohmann::json &j, EmbedImageData &m);
};

struct EmbedThumbnailData {
    std::optional<std::string> URL;
    std::optional<std::string> ProxyURL;
    std::optional<int> Height;
    std::optional<int> Width;

    friend void to_json(nlohmann::json &j, const EmbedThumbnailData &m);
    friend void from_json(const nlohmann::json &j, EmbedThumbnailData &m);
};

struct EmbedVideoData {
    std::optional<std::string> URL;
    std::optional<int> Height;
    std::optional<int> Width;

    friend void to_json(nlohmann::json &j, const EmbedVideoData &m);
    friend void from_json(const nlohmann::json &j, EmbedVideoData &m);
};

struct EmbedProviderData {
    std::optional<std::string> Name;
    std::optional<std::string> URL; // null

    friend void to_json(nlohmann::json &j, const EmbedProviderData &m);
    friend void from_json(const nlohmann::json &j, EmbedProviderData &m);
};

struct EmbedAuthorData {
    std::optional<std::string> Name;
    std::optional<std::string> URL;
    std::optional<std::string> IconURL;
    std::optional<std::string> ProxyIconURL;

    friend void to_json(nlohmann::json &j, const EmbedAuthorData &m);
    friend void from_json(const nlohmann::json &j, EmbedAuthorData &m);
};

struct EmbedFieldData {
    std::string Name;
    std::string Value;
    std::optional<bool> Inline;

    friend void to_json(nlohmann::json &j, const EmbedFieldData &m);
    friend void from_json(const nlohmann::json &j, EmbedFieldData &m);
};

struct EmbedData {
    std::optional<std::string> Title;
    std::optional<std::string> Type;
    std::optional<std::string> Description;
    std::optional<std::string> URL;
    std::optional<std::string> Timestamp;
    std::optional<int> Color;
    std::optional<EmbedFooterData> Footer;
    std::optional<EmbedImageData> Image;
    std::optional<EmbedThumbnailData> Thumbnail;
    std::optional<EmbedVideoData> Video;
    std::optional<EmbedProviderData> Provider;
    std::optional<EmbedAuthorData> Author;
    std::optional<std::vector<EmbedFieldData>> Fields;

    friend void to_json(nlohmann::json &j, const EmbedData &m);
    friend void from_json(const nlohmann::json &j, EmbedData &m);
};

struct AttachmentData {
    Snowflake ID;
    std::string Filename;
    int Bytes;
    std::string URL;
    std::string ProxyURL;
    std::optional<int> Height; // null
    std::optional<int> Width;  // null

    friend void to_json(nlohmann::json &j, const AttachmentData &m);
    friend void from_json(const nlohmann::json &j, AttachmentData &m);
};

struct MessageReferenceData {
    std::optional<Snowflake> MessageID;
    std::optional<Snowflake> ChannelID;
    std::optional<Snowflake> GuildID;

    friend void from_json(const nlohmann::json &j, MessageReferenceData &m);
    friend void to_json(nlohmann::json &j, const MessageReferenceData &m);
};

struct ReactionData {
    int Count;
    bool HasReactedWith;
    Emoji Emoji;

    friend void from_json(const nlohmann::json &j, ReactionData &m);
    friend void to_json(nlohmann::json &j, const ReactionData &m);
};

struct Message {
    Snowflake ID;
    Snowflake ChannelID;
    std::optional<Snowflake> GuildID;
    User Author;
    // std::optional<GuildMember> Member;
    std::string Content;
    std::string Timestamp;
    std::string EditedTimestamp; // null
    bool IsTTS;
    bool DoesMentionEveryone;
    std::vector<User> Mentions; // currently discarded in store
    // std::vector<Role> MentionRoles;
    // std::optional<std::vector<ChannelMentionData>> MentionChannels;
    std::vector<AttachmentData> Attachments;
    std::vector<EmbedData> Embeds;
    std::optional<std::vector<ReactionData>> Reactions;
    std::optional<std::string> Nonce;
    bool IsPinned;
    std::optional<Snowflake> WebhookID;
    MessageType Type;
    // std::optional<MessageActivityData> Activity;
    // std::optional<MessageApplicationData> Application;
    std::optional<MessageReferenceData> MessageReference;
    std::optional<MessageFlags> Flags = MessageFlags::NONE;
    std::optional<std::vector<Sticker>> Stickers;

    friend void from_json(const nlohmann::json &j, Message &m);
    void from_json_edited(const nlohmann::json &j); // for MESSAGE_UPDATE

    // custom fields to track changes
    void SetDeleted();
    void SetEdited();
    bool IsDeleted() const;
    bool IsEdited() const;

private:
    bool m_deleted = false;
    bool m_edited = false;
};
