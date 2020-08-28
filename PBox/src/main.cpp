
#include <Pandora/Core.h>

// To compile, make sure you use --box-builder

using namespace pd;

const ConColor SELF_COLOR = ConColor::Cyan;
const ConColor ERROR_COLOR = ConColor::Red;
const ConColor OPTION_COLOR = ConColor::Yellow;

enum class Operation {
    None,
    Build,
    Unpack,
    List,
    Query,
    New
};

struct Options {
    Operation op = Operation::None;
    String archiveName;

    String unpackPath;
    String configPath;
    String queryName;
};

void ShowHelp(char* path);
void List(Options& options);
void Build(Options& options);
void Unpack(Options& options);
void Query(Options& options);
void New(Options& options);

int main(int argc, char** argv) {

    if (argc == 1) {
        ShowHelp(argv[0]);
        return 0;
    }

    Options options;

    // Parse options
    for (int i = 1; i < argc; i++) {
        // Can only be the first option
        if (i == 1 && strcmp(argv[i], "--no-color") == 0) {
            console.SetColorEnabled(false);
            continue;
        }

        // Parse the operation and archive name
        if (options.op == Operation::None) {
            if (strcmp(argv[i], "--help") == 0) {
                // Help specified, ignore all other options
                ShowHelp(argv[0]);
                return 0;
            } else if (strcmp(argv[i], "--list") == 0) {
                options.op = Operation::List;
            } else if (strcmp(argv[i], "--build") == 0) {
                options.op = Operation::Build;
            } else if (strcmp(argv[i], "--unpack") == 0) {
                options.op = Operation::Unpack;
            } else if (strcmp(argv[i], "--query") == 0) {
                options.op = Operation::Query;
            } else if (strcmp(argv[i], "--new") == 0) {
                options.op = Operation::New;
            } else {
                console.Log("[{}Box Error{}] No valid option supplied, see '{}{}{} {}--help{}' for options",
                    ERROR_COLOR, ConColor::White, SELF_COLOR, argv[0], ConColor::White, OPTION_COLOR, ConColor::White);
                return 1;
            }

            // Every option requires an archive name
            if (i + 1 >= argc) {
                console.Log("[{}Box Error{}] Expected argument <archive>, see '{}{}{} {}--help{}' for options",
                    ERROR_COLOR, ConColor::White, SELF_COLOR, argv[0], ConColor::White, OPTION_COLOR, ConColor::White);
                return 2;
            }

            i += 1;
            options.archiveName.Set(argv[i]);
            continue;
        }

        // Parse the op-specific arguments
        switch (options.op) {
            case Operation::List:
                console.Log("[{}Box Error{}] Too many arguments for {}--list{}, see '{}{}{} {}--help{}' for options",
                    ERROR_COLOR, ConColor::White, OPTION_COLOR, ConColor::White, SELF_COLOR, argv[0], ConColor::White, OPTION_COLOR, ConColor::White);
                return 3;

            case Operation::Build:
                options.configPath.Set(argv[i]);

                if (i + 1 < argc) {
                    console.Log("[{}Box Error{}] Too many arguments for {}--build{}, see '{}{}{} {}--help{}' for options",
                        ERROR_COLOR, ConColor::White, OPTION_COLOR, ConColor::White, SELF_COLOR, argv[0], ConColor::White, OPTION_COLOR, ConColor::White);
                    return 3;
                }
                break;

            case Operation::Unpack:
                options.unpackPath.Set(argv[i]);

                if (options.unpackPath.Back() != '/') {
                    options.unpackPath.Append('/');
                }

                if (i + 1 < argc) {
                    console.Log("[{}Box Error{}] Too many arguments for {}--unpack{}, see '{}{}{} {}--help{}' for options",
                        ERROR_COLOR, ConColor::White, OPTION_COLOR, ConColor::White, SELF_COLOR, argv[0], ConColor::White, OPTION_COLOR, ConColor::White);
                    return 3;
                }
                break;

            case Operation::Query:
                options.queryName.Set(argv[i]);

                if (i + 1 < argc) {
                    console.Log("[{}Box Error{}] Too many arguments for {}--query{}, see '{}{}{} {}--help{}' for options",
                        ERROR_COLOR, ConColor::White, OPTION_COLOR, ConColor::White, SELF_COLOR, argv[0], ConColor::White, OPTION_COLOR, ConColor::White);
                    return 3;
                }
                break;

            case Operation::New:
                console.Log("[{}Box Error{}] Too many arguments for {}--new{}, see '{}{}{} {}--help{}' for options",
                    ERROR_COLOR, ConColor::White, OPTION_COLOR, ConColor::White, SELF_COLOR, argv[0], ConColor::White, OPTION_COLOR, ConColor::White);
                return 3;
        }
    }

    // Check if we specified a config file
    if (options.op == Operation::Build && options.configPath.Count() == 0) {
        console.Log("[{}Box Error{}] No config file specified for {}--build{}, see '{}{}{} {}--help{}' for options",
            ERROR_COLOR, ConColor::White, OPTION_COLOR, ConColor::White, SELF_COLOR, argv[0], ConColor::White, OPTION_COLOR, ConColor::White);
        return 3;
    }

    // Check if we specified an output path
    if (options.op == Operation::Unpack && options.unpackPath.Count() == 0) {
        console.Log("[{}Box Error{}] No unpack path specified for {}--unpack{}, see '{}{}{} {}--help{}' for options",
            ERROR_COLOR, ConColor::White, OPTION_COLOR, ConColor::White, SELF_COLOR, argv[0], ConColor::White, OPTION_COLOR, ConColor::White);
        return 3;
    }

    // Check if we specified a config file
    if (options.op == Operation::Query && options.queryName.Count() == 0) {
        console.Log("[{}Box Error{}] No resource name specified for {}--query{}, see '{}{}{} {}--help{}' for options",
            ERROR_COLOR, ConColor::White, OPTION_COLOR, ConColor::White, SELF_COLOR, argv[0], ConColor::White, OPTION_COLOR, ConColor::White);
        return 3;
    }

    // Do it!
    switch (options.op) {
        case Operation::List: List(options); break;
        case Operation::Build: Build(options); break;
        case Operation::Unpack: Unpack(options); break;
        case Operation::Query: Query(options); break;
        case Operation::New: New(options); break;
    }

    return 0;
}

// File constants
const byte FILE_MAGIC_RAW[] = { 'B', 'O', 'X', '\n' };
const Slice<byte> FILE_MAGIC = Slice<byte>((byte*)FILE_MAGIC_RAW, sizeof(FILE_MAGIC_RAW));
const byte BOX_VERSION = 1;
const byte BOX_SUPPORTED_VERSION = 1;

void ShowHelp(char* path) {
    console.Log("Usage: {}{}{} [--no-color] <option> <arguments...>\n", SELF_COLOR, path, ConColor::White);
    console.Log("  {}--help{}                      | Displays this help screen\n", OPTION_COLOR, ConColor::White);
    console.Log("  {}--list{}   <archive>          | Lists all resources in .box file <archive>\n", OPTION_COLOR, ConColor::White);
    console.Log("  {}--build{}  <archive> <config> | Builds .box file <archive> from config at path <config>\n", OPTION_COLOR, ConColor::White);
    console.Log("  {}--unpack{} <archive> <path>   | Unpacks the <archive> .box into <path>\n", OPTION_COLOR, ConColor::White);
    console.Log("  {}--query{}  <archive> <name>   | Queries info for the resource called <name> in .box file <archive>\n", OPTION_COLOR, ConColor::White);
    console.Log("  {}--new{}    <archive>          | Creates a new config file called <archive>\n", OPTION_COLOR, ConColor::White);
}

void List(Options& options) {
    Box box;
    if (!box.Load(options.archiveName)) {
        console.Log("[{}Box Error{}] Failed to load .box file\n",
            ERROR_COLOR, ConColor::White);
        return;
    }

    Slice<BoxHeader> headers = box.GetHeaders();

    for (int i = 0; i < headers.Count(); i++) {
        console.Log(" > {}'{}'{}\t({})\n", ConColor::Yellow, headers[i].name, ConColor::White, RESOURCE_TYPE_NAMES[(int)headers[i].type]);
    }
}

void Build(Options& options) {
    if (!BuildBoxFromConfig(options.archiveName, options.configPath, true)) {
        console.Log("[{}Box Error{}] could not build box {#} from config {#}",
                ERROR_COLOR, ConColor::White, options.archiveName, options.configPath);
    }
}

void UnpackResource(Box& box, BoxHeader& header, StringView path) {
    switch (header.type) {
        case ResourceType::Binary: {
            Array<byte> bytes;
            box.GetResourceData(header.name, bytes);

            CreateFolder(GetFolder(path));
            WriteEntireFile(path, bytes);
            break;
        }

        default:
            console.Log("[{}Box Error{}] Resource type {} not yet supported\n",
                ERROR_COLOR, ConColor::White, (int)header.type);
            return;
    }
}

void Unpack(Options& options) {
    Box box;
    if (!box.Load(options.archiveName)) {
        console.Log("[{}Box Error{}] Failed to load .box file\n",
            ERROR_COLOR, ConColor::White);
        return;
    }

    Slice<BoxHeader> headers = box.GetHeaders();

    for (int i = 0; i < headers.Count(); i++) {
        console.Log(" > {}{}{}\t({})\n", ConColor::Yellow, headers[i].name, ConColor::White, RESOURCE_TYPE_NAMES[(int)headers[i].type]);
    }

    // Create target directory
    if (!FolderExists(options.unpackPath)) {
        CreateFolder(options.unpackPath);
    }

    for (int i = 0; i < headers.Count(); i++) {
        console.Log("[{}Unpack{}] Unpacking file '{}'...\n", ConColor::Green, ConColor::White, headers[i].name);

        String outputPath;
        outputPath.Set(options.unpackPath);
        outputPath.Append(headers.Data()[i].name);

        UnpackResource(box, headers.Data()[i], outputPath);
    }

    console.Log("[{}Unpack{}] Done!\n", ConColor::Green, ConColor::White);
}

void Query(Options& options) {
    Box box;
    if (!box.Load(options.archiveName)) {
        console.Log("[{}Box Error{}] Failed to load .box file\n",
            ERROR_COLOR, ConColor::White);
        return;
    }

    BoxHeader* header = box.GetResourceHeader(options.queryName);

    if (!header) {
        console.Log("[{}Box Error{}] Resource '{}' does not exist\n",
            ERROR_COLOR, ConColor::White, options.queryName);
        return;
    }

    console.Log("  name:\t{}'{}'{}\n", ConColor::Yellow, options.queryName, ConColor::White);

    // Get sizes and make them nice and reable
    f64 compressedSize = (f64)box.GetCompressedSize(options.queryName);
    f64 uncompressedSize = (f64)box.GetUncompressedSize(options.queryName);

    char* compressedSizeName = "bytes";
    char* uncompressedSizeName = "bytes";

    auto makeReadable = [](f64 size, char** name) {
        if (size >= 1024.0) {
            size /= 1024.0;
            *name = "KiB";
        }
        if (size >= 1024.0) {
            size /= 1024.0;
            *name = "MiB";
        }
        if (size >= 1024.0) {
            size /= 1024.0;
            *name = "GiB";
        }
        return size;
    };

    f64 readableCompressedSize = makeReadable(compressedSize, &compressedSizeName);
    f64 readableUncompressedSize = makeReadable(uncompressedSize, &uncompressedSizeName);

    console.Log("  size:\t");

    console.Log("{}{.2} {}{} (c){}", ConColor::Yellow, readableCompressedSize, compressedSizeName, ConColor::Grey, ConColor::White);
    console.Log(" - ");
    console.Log("{}{.2} {}{} (u){}", ConColor::Yellow, readableUncompressedSize, uncompressedSizeName, ConColor::Grey, ConColor::White);
    console.Log(" | {.2}:1 ratio\n", uncompressedSize / compressedSize);

    // Print resource-specific data
    console.Log("  type:\t{}{}{}\n", ConColor::Yellow, RESOURCE_TYPE_NAMES[(int)header->type], ConColor::White);
    {
        Array<byte> out;
        box.GetResourceData(options.queryName, out);

        MemoryStream memory(out);

        switch (header->type) {
            case ResourceType::Texture: {
                int x, y;
                TextureFiltering filtering;
                TextureWrapping wrapping;

                if (memory.Read(&filtering) != sizeof(filtering)) {
                    console.Log("[{}Box Error{}] Unexpected end-of-stream\n",
                        ERROR_COLOR, ConColor::White, options.queryName);
                    return;
                }

                if (memory.Read(&wrapping) != sizeof(wrapping)) {
                    console.Log("[{}Box Error{}] Unexpected end-of-stream\n",
                        ERROR_COLOR, ConColor::White, options.queryName);
                    return;
                }

                if (memory.Read(&x) != sizeof(x)) {
                    console.Log("[{}Box Error{}] Unexpected end-of-stream\n",
                        ERROR_COLOR, ConColor::White, options.queryName);
                    return;
                }

                if (memory.Read(&y) != sizeof(y)) {
                    console.Log("[{}Box Error{}] Unexpected end-of-stream\n",
                        ERROR_COLOR, ConColor::White, options.queryName);
                    return;
                }

                console.Log("  filter:\t{}{}{}\n",
                    ConColor::Yellow, filtering, ConColor::White, ConColor::Yellow, y, ConColor::White);
                console.Log("  wrapping:\t{}{}{}\n",
                    ConColor::Yellow, wrapping, ConColor::White, ConColor::Yellow, y, ConColor::White);
                console.Log("  dimensions:\t{}{}{}x{}{}{}\n",
                    ConColor::Yellow, x, ConColor::White, ConColor::Yellow, y, ConColor::White);
                break;
            }

            case ResourceType::Mesh: {
                u32 vc, ic;

                if (memory.Read(&vc) != sizeof(vc)) {
                    console.Log("[{}Box Error{}] Unexpected end-of-stream\n",
                        ERROR_COLOR, ConColor::White, options.queryName);
                    return;
                }

                if (memory.Read(&ic) != sizeof(ic)) {
                    console.Log("[{}Box Error{}] Unexpected end-of-stream\n",
                        ERROR_COLOR, ConColor::White, options.queryName);
                    return;
                }

                console.Log("  vertices:\t{}{}{}\n", ConColor::Yellow, vc, ConColor::White);
                console.Log("  indices:\t{}{}{}\n", ConColor::Yellow, ic, ConColor::White);
                break;
            }

            case ResourceType::Shader: {
                byte backendCount;
                if (memory.ReadByte(&backendCount) != 1) {
                    console.Log("[{}Box Error{}] Unexpected end-of-stream\n",
                        ERROR_COLOR, ConColor::White, options.queryName);
                    return;
                }

                console.Log("  sources:\t{}{}{}\n", ConColor::Yellow, backendCount, ConColor::White);

                for (int i = 0; i < (int)backendCount; i++) {
                    u32 vertexSize;
                    u32 pixelSize;
                    VideoBackend backend = VideoBackend::Count;

                    if (memory.Read(&backend) != sizeof(backend)) {
                        console.Log("[{}Box Error{}] Unexpected end-of-stream\n",
                            ERROR_COLOR, ConColor::White, options.queryName);
                        return;
                    }

                    if (memory.Read(&vertexSize) != sizeof(vertexSize)) {
                        console.Log("[{}Box Error{}] Unexpected end-of-stream\n",
                            ERROR_COLOR, ConColor::White, options.queryName);
                        return;
                    }
                    memory.Seek(vertexSize);

                    if (memory.Read(&pixelSize) != sizeof(pixelSize)) {
                        console.Log("[{}Box Error{}] Unexpected end-of-stream\n",
                            ERROR_COLOR, ConColor::White, options.queryName);
                        return;
                    }
                    memory.Seek(pixelSize);

                    char* vertexSizeName = "bytes";
                    char* pixelSizeName = "bytes";

                    f64 readableVertexSize = makeReadable((f64)vertexSize, &vertexSizeName);
                    f64 readablePixelSize = makeReadable((f64)pixelSize, &pixelSizeName);

                    console.Log("    backend:\t{}{}{}\n", ConColor::Yellow, backend, ConColor::White);
                    console.Log("    size:\t");
                    console.Log("{}{.2} {} {}(v)",
                        ConColor::Yellow, readableVertexSize, vertexSizeName, ConColor::Grey);

                    console.Log("{} - ", ConColor::White);

                    console.Log("{}{.2} {} {}(p)",
                        ConColor::Yellow, readablePixelSize, pixelSizeName, ConColor::Grey);

                    console.Log("{}\n", ConColor::White);
                }

                break;
            }

            case ResourceType::Audio: {
                f64 lengthInSeconds;
                u32 sampleRate;
                u16 channels;
                u32 sampleCount;

                if (memory.Read<f64>(&lengthInSeconds) != sizeof(f64)) {
                    console.Log("[{}Box Error{}] Unexpected end-of-stream\n",
                        ERROR_COLOR, ConColor::White, options.queryName);
                    return;
                }

                if (memory.Read<u32>(&sampleRate) != sizeof(u32)) {
                    console.Log("[{}Box Error{}] Unexpected end-of-stream\n",
                        ERROR_COLOR, ConColor::White, options.queryName);
                    return;
                }

                if (memory.Read<u16>(&channels) != sizeof(u16)) {
                    console.Log("[{}Box Error{}] Unexpected end-of-stream\n",
                        ERROR_COLOR, ConColor::White, options.queryName);
                    return;
                }

                if (memory.Read<u32>(&sampleCount) != sizeof(u32)) {
                    console.Log("[{}Box Error{}] Unexpected end-of-stream\n",
                        ERROR_COLOR, ConColor::White, options.queryName);
                    return;
                }

                console.Log("  playtime:\t{}{#} seconds{}\n", ConColor::Yellow, lengthInSeconds, ConColor::White);
                console.Log("  sample rate:\t{}{#}{}\n", ConColor::Yellow, sampleRate, ConColor::White);
                console.Log("  channels:\t{}{}{}\n", ConColor::Yellow, channels, ConColor::White);
                console.Log("  samples:\t{}{}{}\n", ConColor::Yellow, sampleCount, ConColor::White);
                break;
            }

        }
    }
}

void New(Options& options) {
    JsonValue config(JsonType::Object);

    {
        JsonValue items(JsonType::Array);
        {
            JsonValue binary(JsonType::Object);
            binary.AddField("name", "ResourceName");
            binary.AddField("type", "Binary");
            binary.AddField("source", "data/asset.bin");
            items.AddElement(binary);
        }
        config.AddField("items", items);
    }

    if (!config.WriteToFile(options.archiveName)) {
        console.Log("[{}Box Error{}] failed to write to file {#}\n", ERROR_COLOR, ConColor::White, options.archiveName);
    }
}
