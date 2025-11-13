#include "MultiMedia.h"

#include "../Utils/Utils.h"
#include "../../../DesktopEditor/graphics/Image.h"
namespace OFD
{
CMultiMedia::CMultiMedia(CXmlReader& oXmlReader, const std::wstring& wsRootPath, IFolder *pFolder)
    : IOFDElement(oXmlReader), m_eType(EType::Image), m_pFolder(pFolder)
{
	if (0 != oXmlReader.GetAttributesCount() && oXmlReader.MoveToFirstAttribute())
	{
		std::string sArgumentName;
		do
		{
			sArgumentName = oXmlReader.GetNameA();

			 if ("Type" == sArgumentName)
			{
				const std::string sValue{oXmlReader.GetTextA()};

				if ("Image" == sValue)
					m_eType = EType::Image;
				else if ("Audio" == sValue)
					m_eType = EType::Audio;
				else if ("Video" == sValue)
					m_eType = EType::Video;
			}
		} while (oXmlReader.MoveToNextAttribute());
	}

	oXmlReader.MoveToElement();

	if (oXmlReader.IsEmptyNode())
		return;

	const int nDepth = oXmlReader.GetDepth();

	while (oXmlReader.ReadNextSiblingNode(nDepth))
	{
		if ("ofd:MediaFile" == oXmlReader.GetNameA())
		{
			const std::wstring wsPath{oXmlReader.GetText2()};

			if (CanUseThisPath(wsPath, wsRootPath))
				m_wsFilePath = CombinePaths(wsRootPath, wsPath);

			break;
		}
	}
}

CMultiMedia::EType CMultiMedia::GetType() const
{
	return m_eType;
}

std::wstring CMultiMedia::GetFilePath() const
{
	return m_wsFilePath;
}

IFolder::IFolderType CMultiMedia::FolderType()const
{
    return m_pFolder->getType();
}
void CMultiMedia::Draw(IRenderer* pRenderer)const
{
    if(m_pFolder->getType() == IFolder::iftFolder){
        pRenderer->DrawImageFromFile(m_wsFilePath, 0, 0, 1, 1);
    }else{
        IFolder::CBuffer* pBuffer=nullptr;
        m_pFolder->read(m_wsFilePath, pBuffer);
        if(pBuffer){
        Aggplus::CImage oImage;
        oImage.Decode(pBuffer->Buffer, pBuffer->Size);
        pRenderer->DrawImage(&oImage, 0, 0, 1, 1);
        RELEASEOBJECT(pBuffer);
        }
    }
}
}
