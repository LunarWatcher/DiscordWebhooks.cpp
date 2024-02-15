# DiscordWebhook.cpp

Simple feed reader without a bot interface

## Archival notice

Under very specific circumstances, line 115 of `main.cpp` results in `channels.json` being written as a blank file, thereby resulting in the feed reader disabling itself the next time it's restarted. I've been unable to identify what causes it to happen, as it seems to be related to some form of race condition (the write function being called just as the machine is shut down?), which makes it a pain in the ass to identify. 

Realistically, this is a consequence of me not using a real database. However, adding a database is tedious, and I just can't be arsed. The system has multiple bugs that a proper RSS parser does not, which is partly caused by only storing the last entry. If that entry is deleted, DiscordWebhooks.cpp assumes _all_ the entries are new and unseen, because date parsing of potentially variable formats fucking sucks so I didn't bother.

I've personally switched to [Miniflux](https://github.com/miniflux/v2/) (which is a proper RSS parser), and using [hazel](https://github.com/LunarWatcher/hazel) as an adapter for its websocket format, though there are plenty of bots purely aimed at posting RSS feeds to Discord. Miniflux has an [open issue](https://github.com/miniflux/v2/issues/1642) for a direct Discord integration, so in the future, it's possible only Miniflux is needed.

In either case, unless you plan to fork and add a database before deploying, I strongly discourage using DiscordWebhooks.cpp for anything (other than maybe as a reference for a similarly simple system).

## Usage

Compile, make a `channels.json` in this format:

```json
[
    {
        "outputs": {"webhook1": "optional message for webook 1; blank if undesired", "webhook2": "", ..., "webhookn": ""},

        "url": "feed URL. Required",
        "username": "Bot name. Optional",
        "lastEntry": "A pubDate in the RSS. Refers to the last seen entry. May be omitted, as the program is capable of populating it on its own. Note that it posts all entries in the feed if it isn't told to stop at a pubDate. This program also assumes all well-formed RSS feeds have a pubDate"
    }, {
        // More entries in the same pattern here
    }
]
```

Run, done \o/
