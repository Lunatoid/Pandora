
#include "Game.h"

#include <Pandora/Entry.h>

pd::App* pd::CreateApp(int argc, char** argv) {
    VideoBackend backend = VideoBackend::OpenGL;

    JsonParseSettings settings;
    settings.allowComments = true;

    JsonValue* json = New<JsonValue>();
    if (json->Parse("config.json", true, settings) && json->Type() == JsonType::Object && json->HasField("backend")) {
        JsonValue back = json->GetField("backend");
        if (back.Type() == JsonType::String) {
            for (int i = 0; i < (int)VideoBackend::Count; i++) {

                if (back.GetString() == pd::VIDEO_BACKEND_NAMES[i]) {
                    backend = (VideoBackend)i;
                    break;
                }
            }
        }
    }

    return New<Game>(argc, argv, backend, json);
}
