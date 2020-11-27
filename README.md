# DiscordWebhook.cpp

Simple feed reader without a bot interface

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
