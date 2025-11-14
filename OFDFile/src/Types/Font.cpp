#include "Font.h"

#include "../Utils/Utils.h"
#include "../../../DesktopEditor/graphics/pro/Fonts.h"
namespace OFD
{
CFont::CFont(CXmlReader& oXmlReader, const std::wstring& wsRootPath, IFolder *pFolder)
	: IOFDElement(oXmlReader),
	  m_wsCharset(L"unicode"), m_bItalic(false), m_bBold(false),
      m_bSerif(false), m_bFixedWidth(false), m_pFolder(pFolder)
{
	if (0 != oXmlReader.GetAttributesCount() && oXmlReader.MoveToFirstAttribute())
	{
		std::string sArgumentName;

		do
		{
			sArgumentName = oXmlReader.GetNameA();

			if ("FontName" == sArgumentName)
				m_wsFontName = oXmlReader.GetText();
			else if ("FamilyName" == sArgumentName)
				m_wsFamilyName = oXmlReader.GetText();
			else if ("Charset" == sArgumentName)
				m_wsCharset = oXmlReader.GetText();
			else if ("Italic" == sArgumentName)
				m_bItalic = oXmlReader.GetBoolean(true);
			else if ("Bold" == sArgumentName)
				m_bBold = oXmlReader.GetBoolean(true);
			else if ("Serif" == sArgumentName)
				m_bSerif = oXmlReader.GetBoolean(true);
			else if ("FixedWidth" == sArgumentName)
				m_bFixedWidth = oXmlReader.GetBoolean(true);
		} while (oXmlReader.MoveToNextAttribute());
	}

	oXmlReader.MoveToElement();

	if (oXmlReader.IsEmptyNode())
		return;

	const int nDepth = oXmlReader.GetDepth();

	while (oXmlReader.ReadNextSiblingNode(nDepth))
	{
		if ("ofd:FontFile" == oXmlReader.GetNameA())
		{
			const std::wstring wsPath{oXmlReader.GetText2()};

			if (CanUseThisPath(wsPath, wsRootPath))
				m_wsFilePath = CombinePaths(wsRootPath, wsPath);

			break;
		}
	}
}

void CFont::Apply(IRenderer* pRenderer) const
{
	if (nullptr == pRenderer)
		return;

	int nFontStyle = 0;

	if (m_bBold)
		nFontStyle |= 0x01;
	if (m_bItalic)
		nFontStyle |= 0x02;

	pRenderer->put_FontStyle(nFontStyle);
	pRenderer->put_FontName(m_wsFontName);
#ifdef BUILDING_WASM_MODULE
    if (!m_wsFilePath.empty()){
        std::wstring wsTempFileName = L"";
        if (NSFonts::NSApplicationFontStream::GetGlobalMemoryStorage())
        {
            IFolder::CBuffer* pBuffer=nullptr;
            m_pFolder->read(m_wsFilePath, pBuffer);
            wsTempFileName = NSFonts::NSApplicationFontStream::GetGlobalMemoryStorage()->GenerateId();
            NSFonts::NSApplicationFontStream::GetGlobalMemoryStorage()->Add(wsTempFileName, pBuffer->Buffer, (LONG)pBuffer->Size, true);
            pRenderer->put_FontPath(wsTempFileName);
            RELEASEOBJECT(pBuffer);
        }
    }
#else
	if (!m_wsFilePath.empty())
		pRenderer->put_FontPath(m_wsFilePath);
#endif
}
}
