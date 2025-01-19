#ifndef MINITELVIEWER_WINOPENVDTFILE_H
#define MINITELVIEWER_WINOPENVDTFILE_H

#include "IHM.h"
#include <string>
#include "imfilebrowser.h"

namespace IHM
{
    class WinOpenVDTFile : public CIHM
    {
    public:
        WinOpenVDTFile(eMinitel& r) : CIHM(r) 
        {
            m_file_browser.SetTitle("title");
            m_file_browser.SetTypeFilters({ ".vdt" });
        }
        void open() { m_file_browser.Open(); }
        void update() override;
        void reset();
        inline const std::string& getFilePath() const { return m_path_file; }

        inline void OnConnect(std::function<bool()>&& on) { m_on_connect = on; }
    private:
        ImGui::FileBrowser m_file_browser;
        bool m_last_connection_failed = false;
        std::string m_path_file;
        std::function<bool()> m_on_connect;
    };
}

#endif //MINITELVIEWER_WINOPENVDTFILE_H

