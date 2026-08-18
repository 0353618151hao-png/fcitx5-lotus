/*
 * SPDX-FileCopyrightText: 2026 Nguyễn Hoàng Kỳ  <nhktmdzhg@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include "lotus-icon-resolver.h"
#include <unistd.h>

namespace fcitx {

    static bool fileReadable(const std::string& path) {
        return !path.empty() && access(path.c_str(), R_OK) == 0;
    }

    static std::string joinPath(const std::string& dir, const std::string& name) {
        if (dir.empty()) return name;
        if (dir.back() == '/') return dir + name;
        return dir + "/" + name;
    }

    std::string resolveLotusIconPath(const std::vector<std::string>& names, const LotusIconSearchPaths& paths) {
        // 1. Probe each candidate name in each system directory.
        //    PNG first: Qt renders PNG natively without plugins; engines with
        //    SVG-only dirs still get SVG because PNG won't exist there.
        for (const auto& name : names) {
            for (const auto& dir : paths.systemDirs) {
                for (const char* ext : {".png", ".svg"}) {
                    const std::string p = joinPath(dir, name + ext);
                    if (fileReadable(p)) return p;
                }
            }
        }

        // 2. Fallback — the compile-time install directory.  Returned even if
        //    unreadable so the caller always gets a stable path.
        return joinPath(paths.fallbackDir, names.front() + ".svg");
    }

} // namespace fcitx
