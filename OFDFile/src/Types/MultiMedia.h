#ifndef MULTIMEDIA_H
#define MULTIMEDIA_H

#include "../IOFDElement.h"
#include "../../../OfficeUtils/src/ZipFolder.h"
#include "../../../DesktopEditor/graphics/IRenderer.h"
namespace OFD
{
class CMultiMedia : public IOFDElement
{
public:
    CMultiMedia(CXmlReader& oXmlReader, const std::wstring& wsRootPath, IFolder *pFolder);

	enum class EType
	{
		Image,
		Audio,
		Video
	};

	EType GetType() const;
	std::wstring GetFilePath() const;
    IFolder::IFolderType FolderType()const;
    void Draw(IRenderer* pRenderer) const;
private:
	EType        m_eType;
	std::wstring m_wsFilePath;
    mutable IFolder     *m_pFolder;
};
}

#endif // MULTIMEDIA_H
