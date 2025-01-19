#include "WinOpenVDTFile.h"
#include <iostream>

using namespace IHM;

void WinOpenVDTFile::reset()
{
    m_last_connection_failed = false;
}

void WinOpenVDTFile::update()
{
    m_file_browser.Display();
    if(m_file_browser.HasSelected())
    {
        m_path_file = m_file_browser.GetSelected().string();
        m_on_connect();
        m_file_browser.ClearSelected();
    }
}