#include "common/utils.hpp"

#include "core/titleLoader.hpp"

// Free the Icon stuff properly.
TitleLoader::~TitleLoader(void)
{
	C3D_TexDelete(mIcon.tex);
}

// Load the Icon Texture of the Title.
static C2D_Image loadTextureIcon(smdh_s* smdh)
{
	C3D_Tex* tex                          = new C3D_Tex;
	static const Tex3DS_SubTexture subt3x = {48, 48, 0.0f, 48 / 64.0f, 48 / 64.0f, 0.0f};
	C3D_TexInit(tex, 64, 64, GPU_RGB565);

	u16* dest = (u16*)tex->data + (64 - 48) * 64;
	u16* src  = (u16*)smdh->bigIconData;
	for (int j = 0; j < 48; j += 8)
	{
		std::copy(src, src + 48 * 8, dest);
		src += 48 * 8;
		dest += 64 * 8;
	}

	return C2D_Image{tex, &subt3x};
}

// Load the available Titles.
bool TitleLoader::loadTitle(u64 id, FS_MediaType media)
{
	bool loadTitle = false;
	mId            = id;
	mMedia         = media;

	smdh_s* smdh = loadSMDH(lowId(), highId(), mMedia);
	if (smdh == NULL)
	{
		return false;
	}

	char unique[12] = {0};
	sprintf(unique, "0x%05X ", (unsigned int)uniqueId());
	mName   = StringUtils::UTF16toUTF8((char16_t*)smdh->applicationTitles[1].shortDescription);

	// Product Code.
	AM_GetTitleProductCode(mMedia, mId, _productCode);
	mProductCode = _productCode;

	loadTitle = true;
	mIcon     = loadTextureIcon(smdh);
	delete smdh;
	return loadTitle;
}

// Other needed stuff for LeafEdit to work.
u32 TitleLoader::highId(void)
{
	return (u32)(mId >> 32);
}

u32 TitleLoader::lowId(void)
{
	return (u32)mId;
}

FS_MediaType TitleLoader::mediaType(void)
{
	return mMedia;
}

C2D_Image TitleLoader::icon(void)
{
	return mIcon;
}

std::string TitleLoader::name(void)
{
	return mName;
}

u32 TitleLoader::uniqueId(void)
{
	return (lowId() >> 8);
}

char *TitleLoader::productCode(void)
{
	return mProductCode;
}