#include "main/files.hpp"

std::string getAssetsFilePath()
{
    CFStringRef appBundle_UTI;
    bool isAppBundle = CFURLCopyResourcePropertyForKey(CFBundleCopyBundleURL(CFBundleGetMainBundle()), kCFURLTypeIdentifierKey, &appBundle_UTI, NULL) && appBundle_UTI && UTTypeConformsTo(appBundle_UTI, kUTTypeApplicationBundle);

    std::string path;
    if (isAppBundle)
    {
        char chPath[PATH_MAX];
        CFURLRef resourcesURL = CFBundleCopyResourcesDirectoryURL(CFBundleGetMainBundle());
        CFURLGetFileSystemRepresentation(resourcesURL, true, (UInt8 *)chPath, PATH_MAX);
        CFRelease(resourcesURL);
        chdir(chPath);

        path = std::string(chPath) + "/";
    }
    else
    {
        std::string currentPath = std::filesystem::current_path().string();
        size_t substrIndex = currentPath.find("build/");
        if (substrIndex != std::string::npos)
            path = currentPath.substr(0, substrIndex) + "assets/";
        else
            path = "assets/";
    }

    return path;
}

std::string getApplicationSupportFilePath(const std::string &appName)
{
    char chPath[PATH_MAX];
    FSRef ref;
    FSFindFolder(kUserDomain, kApplicationSupportFolderType, kCreateFolder, &ref);
    FSRefMakePath(&ref, (UInt8 *)&chPath, PATH_MAX);

    std::string path = std::string(chPath) + "/" + appName + "/";

    if (!std::filesystem::is_directory(path))
        std::filesystem::create_directory(path);

    return path;
}

std::map<std::string, std::string> getDefaultFilePaths()
{
    std::map<std::string, std::string> result;

    char chPath[PATH_MAX];
    FSRef ref;

    FSFindFolder(kUserDomain, kDocumentsFolderType, kDontCreateFolder, &ref);
    FSRefMakePath(&ref, (UInt8 *)&chPath, PATH_MAX);
    result["Documents"] = std::string(chPath);

    FSFindFolder(kUserDomain, kDownloadsFolderType, kDontCreateFolder, &ref);
    FSRefMakePath(&ref, (UInt8 *)&chPath, PATH_MAX);
    result["Downloads"] = std::string(chPath);

    FSFindFolder(kUserDomain, kMusicDocumentsFolderType, kDontCreateFolder, &ref);
    FSRefMakePath(&ref, (UInt8 *)&chPath, PATH_MAX);
    result["Music"] = std::string(chPath);

    FSFindFolder(kUserDomain, kPictureDocumentsFolderType, kDontCreateFolder, &ref);
    FSRefMakePath(&ref, (UInt8 *)&chPath, PATH_MAX);
    result["Pictures"] = std::string(chPath);

    FSFindFolder(kUserDomain, kDesktopFolderType, kDontCreateFolder, &ref);
    FSRefMakePath(&ref, (UInt8 *)&chPath, PATH_MAX);
    result["Desktop"] = std::string(chPath);

    FSFindFolder(kUserDomain, kMovieDocumentsFolderType, kDontCreateFolder, &ref);
    FSRefMakePath(&ref, (UInt8 *)&chPath, PATH_MAX);
    result["Movies"] = std::string(chPath);

    FSFindFolder(kUserDomain, kApplicationsFolderType, kDontCreateFolder, &ref);
    FSRefMakePath(&ref, (UInt8 *)&chPath, PATH_MAX);
    result["Applications"] = std::string(chPath);

    FSFindFolder(kUserDomain, kPublicFolderType, kDontCreateFolder, &ref);
    FSRefMakePath(&ref, (UInt8 *)&chPath, PATH_MAX);
    result["Public"] = std::string(chPath);

    result["Root"] = std::filesystem::current_path().root_path().string();
    return result;
}