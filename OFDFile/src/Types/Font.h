#ifndef FONT_H
#define FONT_H

#include "../IOFDElement.h"

#include "../../../DesktopEditor/graphics/IRenderer.h"
#include "../../../OfficeUtils/src/ZipFolder.h"
namespace OFD
{
class CFont : public IOFDElement
{
public:
    CFont(CXmlReader& oXmlReader, const std::wstring& wsRootPath, IFolder *pFolder);

	void Apply(IRenderer* pRenderer) const;
private:
	std::wstring m_wsFontName;
	std::wstring m_wsFamilyName;
	std::wstring m_wsCharset;
	bool         m_bItalic;
	bool         m_bBold;
	bool         m_bSerif;
	bool         m_bFixedWidth;

	std::wstring m_wsFilePath;
    mutable IFolder     *m_pFolder;
};
}

#endif // FONT_H
