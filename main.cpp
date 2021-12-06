#include <Windows.h>

#include <stdio.h>
#include <stdlib.h>

#include <vector>
#include <string>

// used https://stackoverflow.com/questions/11515548/how-to-scan-all-files-in-a-folder-including-subfolders-in-vc-2008 from Rango



struct SearchFile
{
    typedef std::vector<std::string> FileNameArray;
    FileNameArray files;

    FileNameArray::iterator begin()
    {
        return files.begin();
    }

    FileNameArray::iterator end()
    {
        return files.end();
    }

    int count() const
    {
        return (int)files.size();
    }

    std::string operator[](int index)
    {
        return files[index];
    }

    void operator()(const std::string &path, const std::string &pattern)
    {
        WIN32_FIND_DATA wfd;
        HANDLE hf;
        std::string findwhat;
        std::vector<std::string> dir;

        findwhat = path + "\\*";  // directory

        hf = FindFirstFile(findwhat.c_str(), &wfd);
        while (hf != INVALID_HANDLE_VALUE)
        {
            if (wfd.cFileName[0] != '.' && (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
            {
                std::string found;

                found = path + "\\" + wfd.cFileName;
                dir.push_back(found);
            }

            if (!FindNextFile(hf, &wfd))
            {
                FindClose(hf);
                hf = INVALID_HANDLE_VALUE;
            }
        }

        findwhat = path + "\\" + pattern;  // files

        hf = FindFirstFile(findwhat.c_str(), &wfd);
        while (hf != INVALID_HANDLE_VALUE)
        {
            if (wfd.cFileName[0] != '.' && !(wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
            {
                std::string found;

                found = path + "\\" + wfd.cFileName;
                files.push_back(found);
            }

            if (!FindNextFile(hf, &wfd))
            {
                FindClose(hf);
                hf = INVALID_HANDLE_VALUE;
            }
        }

        // continue with directories
        for (std::vector<std::string>::iterator it = dir.begin(); it != dir.end(); ++it)
            this->operator()(*it, pattern);
    }
};



void actions_for_file(std::string file_w_rel_path)
{
    printf("del %s\n", file_w_rel_path.c_str());
    DeleteFile(file_w_rel_path.c_str());
}

void scan_for_ext(std::string files_exts)
{

    SearchFile sf;

    // get all .jpg files in current dir
    sf(".", files_exts);

    for (int i = 0; i != sf.count(); ++i)
    {
        actions_for_file(sf[i].c_str());
        //printf("%s\n", sf[i].c_str());
    }


}

int wmain()
{

    scan_for_ext("*.#0?");
    scan_for_ext("*.ilc");
    scan_for_ext("*.ild");
    scan_for_ext("*.ilf");
    scan_for_ext("*.ils");
    scan_for_ext("*.obj");
    scan_for_ext("*.pch");
    scan_for_ext("*.tds");


    {
        printf("Done! enter any key\n");
        getchar();
    }

    return 0;
}
