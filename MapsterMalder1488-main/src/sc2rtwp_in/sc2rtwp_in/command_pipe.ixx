module;

#include <common/win_headers.h>

export module injected.command_pipe;

import std;
import common;
import injected.logger;
import injected.app;

using ReportFunction = void (*)(unsigned int*, unsigned int, char*);

void performAction(const std::vector<u32>& handles)
{
    auto& app = App::instance();
    auto reportFunc = reinterpret_cast<ReportFunction>(reinterpret_cast<char*>(app.imagebase()) + 0x21A8550);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> len_dist(1, 8);
    std::uniform_int_distribution<> char_dist('a', 'z');

    for (auto handle : handles)
    {
        for (unsigned int category : {0x64, 0x65, 0x66})
        {
            std::string desc;
            int len = len_dist(gen);
            desc.reserve(len);
            for (int i = 0; i < len; ++i)
            {
                desc += static_cast<char>(char_dist(gen));
            }
            
            Log::msg("Reporting map handle {} with category {:#x} and description '{}'", handle, category, desc);
            reportFunc(const_cast<u32*>(&handle), category, desc.data());
        }
    }
}

export class CommandPipeServer : Noncopyable
{
public:
    CommandPipeServer()
    {
        mPipeName = std::format(R"(\\.\pipe\sc2rtwp_cmd_{})", GetCurrentProcessId());
    }

    void run()
    {
        char buffer[4096]; // Increased buffer size
        while (true)
        {
            Log::msg("Command pipe: waiting for connection on {}", mPipeName);
            SmartHandle pipe = CreateNamedPipeA(mPipeName.c_str(), PIPE_ACCESS_INBOUND, PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT, PIPE_UNLIMITED_INSTANCES, sizeof(buffer), sizeof(buffer), 0, nullptr);
            if (!pipe)
            {
                Log::msg("Command pipe: CreateNamedPipeA failed, error {}", GetLastError());
                std::this_thread::sleep_for(std::chrono::seconds(1));
                continue;
            }

            const auto connected = ConnectNamedPipe(pipe, nullptr);
            if (!connected && GetLastError() != ERROR_PIPE_CONNECTED)
            {
                Log::msg("Command pipe: ConnectNamedPipe failed, error {}", GetLastError());
                continue;
            }
            
            Log::msg("Command pipe: injector connected.");

            DWORD bytesRead = 0;
            while (ReadFile(pipe, buffer, sizeof(buffer) - 1, &bytesRead, nullptr) && bytesRead > 0)
            {
                buffer[bytesRead] = '\0';
                std::string_view command_str(buffer);
                Log::msg("Command pipe: received command '{}'", command_str);

                if (command_str.starts_with("action"))
                {
                    std::vector<u32> handles;
                    auto pos = command_str.find(':');
                    if (pos != std::string_view::npos) {
                        std::string_view handles_sv = command_str.substr(pos + 1);
                        std::string current_handle;
                        for (char c : handles_sv) {
                            if (c == ',') {
                                if (!current_handle.empty()) {
                                    handles.push_back(std::stoul(current_handle));
                                    current_handle.clear();
                                }
                            } else {
                                current_handle += c;
                            }
                        }
                        if (!current_handle.empty()) {
                            handles.push_back(std::stoul(current_handle));
                        }
                    }
                    
                    Log::msg("Command pipe: performing action with {} handles!", handles.size());
                    performAction(handles);
                }
            }
            
            Log::msg("Command pipe: injector disconnected.");
            DisconnectNamedPipe(pipe);
        }
    }

private:
    std::string mPipeName;
};
