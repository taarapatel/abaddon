#pragma once
#include "../util.hpp"
#include "objects.hpp"
#include <unordered_map>
#include <unordered_set>
#include <mutex>
#include <filesystem>
#include <sqlite3.h>

#ifdef GetMessage // fuck you windows.h
    #undef GetMessage
#endif

class Store {
public:
    Store(bool mem_store = false);
    ~Store();

    bool IsValid() const;

    void SetUser(Snowflake id, const User &user);
    void SetChannel(Snowflake id, const Channel &chan);
    void SetGuild(Snowflake id, const Guild &guild);
    void SetRole(Snowflake id, const Role &role);
    void SetMessage(Snowflake id, const Message &message);
    void SetGuildMember(Snowflake guild_id, Snowflake user_id, const GuildMember &data);
    void SetPermissionOverwrite(Snowflake channel_id, Snowflake id, const PermissionOverwrite &perm);
    void SetEmoji(Snowflake id, const Emoji &emoji);

    // slap const on everything even tho its not *really* const

    std::optional<Channel> GetChannel(Snowflake id) const;
    std::optional<Emoji> GetEmoji(Snowflake id) const;
    std::optional<Guild> GetGuild(Snowflake id) const;
    std::optional<GuildMember> GetGuildMember(Snowflake guild_id, Snowflake user_id) const;
    std::optional<Message> GetMessage(Snowflake id) const;
    std::optional<PermissionOverwrite> GetPermissionOverwrite(Snowflake channel_id, Snowflake id) const;
    std::optional<Role> GetRole(Snowflake id) const;
    std::optional<User> GetUser(Snowflake id) const;

    void ClearGuild(Snowflake id);
    void ClearChannel(Snowflake id);

    using users_type = std::unordered_map<Snowflake, User>;
    using channels_type = std::unordered_map<Snowflake, Channel>;
    using guilds_type = std::unordered_map<Snowflake, Guild>;
    using roles_type = std::unordered_map<Snowflake, Role>;
    using messages_type = std::unordered_map<Snowflake, Message>;
    using members_type = std::unordered_map<Snowflake, std::unordered_map<Snowflake, GuildMember>>;                       // [guild][user]
    using permission_overwrites_type = std::unordered_map<Snowflake, std::unordered_map<Snowflake, PermissionOverwrite>>; // [channel][user/role]
    using emojis_type = std::unordered_map<Snowflake, Emoji>;

    const std::unordered_set<Snowflake> &GetChannels() const;
    const std::unordered_set<Snowflake> &GetGuilds() const;

    void ClearAll();

    void BeginTransaction();
    void EndTransaction();

private:
    std::unordered_set<Snowflake> m_channels;
    std::unordered_set<Snowflake> m_guilds;

    bool CreateTables();
    bool CreateStatements();
    void Cleanup();

    template<typename T>
    void Bind(sqlite3_stmt *stmt, int index, const std::optional<T> &opt) const;
    void Bind(sqlite3_stmt *stmt, int index, int num) const;
    void Bind(sqlite3_stmt *stmt, int index, uint64_t num) const;
    void Bind(sqlite3_stmt *stmt, int index, const std::string &str) const;
    void Bind(sqlite3_stmt *stmt, int index, bool val) const;
    void Bind(sqlite3_stmt *stmt, int index, std::nullptr_t) const;
    bool RunInsert(sqlite3_stmt *stmt);
    bool FetchOne(sqlite3_stmt *stmt) const;
    template<typename T>
    void Get(sqlite3_stmt *stmt, int index, std::optional<T> &out) const;
    void Get(sqlite3_stmt *stmt, int index, int &out) const;
    void Get(sqlite3_stmt *stmt, int index, uint64_t &out) const;
    void Get(sqlite3_stmt *stmt, int index, std::string &out) const;
    void Get(sqlite3_stmt *stmt, int index, bool &out) const;
    void Get(sqlite3_stmt *stmt, int index, Snowflake &out) const;
    bool IsNull(sqlite3_stmt *stmt, int index) const;
    void Reset(sqlite3_stmt *stmt) const;

    std::filesystem::path m_db_path;
    mutable sqlite3 *m_db;
    mutable int m_db_err;
    mutable sqlite3_stmt *m_set_user_stmt;
    mutable sqlite3_stmt *m_get_user_stmt;
    mutable sqlite3_stmt *m_set_perm_stmt;
    mutable sqlite3_stmt *m_get_perm_stmt;
    mutable sqlite3_stmt *m_set_msg_stmt;
    mutable sqlite3_stmt *m_get_msg_stmt;
    mutable sqlite3_stmt *m_set_role_stmt;
    mutable sqlite3_stmt *m_get_role_stmt;
    mutable sqlite3_stmt *m_set_emote_stmt;
    mutable sqlite3_stmt *m_get_emote_stmt;
    mutable sqlite3_stmt *m_set_member_stmt;
    mutable sqlite3_stmt *m_get_member_stmt;
    mutable sqlite3_stmt *m_set_guild_stmt;
    mutable sqlite3_stmt *m_get_guild_stmt;
    mutable sqlite3_stmt *m_set_chan_stmt;
    mutable sqlite3_stmt *m_get_chan_stmt;
};

template<typename T>
inline void Store::Bind(sqlite3_stmt *stmt, int index, const std::optional<T> &opt) const {
    if (opt.has_value())
        Bind(stmt, index, *opt);
    else
        sqlite3_bind_null(stmt, index);
}

template<typename T>
inline void Store::Get(sqlite3_stmt *stmt, int index, std::optional<T> &out) const {
    if (sqlite3_column_type(stmt, index) == SQLITE_NULL)
        out = std::nullopt;
    else {
        T v;
        Get(stmt, index, v);
        out = std::optional<T>(v);
    }
}
