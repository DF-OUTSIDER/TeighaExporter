#
# PDFium library
#

set (public_files
           public/cpp/fpdf_deleters.h
           public/fpdf_annot.h
           public/fpdf_attachment.h
           public/fpdf_catalog.h
           public/fpdf_dataavail.h
           public/fpdf_doc.h
           public/fpdf_edit.h
           public/fpdf_ext.h
           public/fpdf_flatten.h
           public/fpdf_formfill.h
           public/fpdf_fwlevent.h
           public/fpdf_ppo.h
           public/fpdf_progressive.h
           public/fpdf_save.h
           public/fpdf_searchex.h
           public/fpdf_structtree.h
           public/fpdf_sysfontinfo.h
           public/fpdf_text.h
           public/fpdf_transformpage.h
           public/fpdfview.h
)
SOURCE_GROUP ("public" FILES ${public_files})

set (fpdfapi_cmaps_files
           core/fpdfapi/cmaps/CNS1/Adobe-CNS1-UCS2_5.cpp
           core/fpdfapi/cmaps/CNS1/B5pc-H_0.cpp
           core/fpdfapi/cmaps/CNS1/B5pc-V_0.cpp
           core/fpdfapi/cmaps/CNS1/CNS-EUC-H_0.cpp
           core/fpdfapi/cmaps/CNS1/CNS-EUC-V_0.cpp
           core/fpdfapi/cmaps/CNS1/ETen-B5-H_0.cpp
           core/fpdfapi/cmaps/CNS1/ETen-B5-V_0.cpp
           core/fpdfapi/cmaps/CNS1/ETenms-B5-H_0.cpp
           core/fpdfapi/cmaps/CNS1/ETenms-B5-V_0.cpp
           core/fpdfapi/cmaps/CNS1/HKscs-B5-H_5.cpp
           core/fpdfapi/cmaps/CNS1/HKscs-B5-V_5.cpp
           core/fpdfapi/cmaps/CNS1/UniCNS-UCS2-H_3.cpp
           core/fpdfapi/cmaps/CNS1/UniCNS-UCS2-V_3.cpp
           core/fpdfapi/cmaps/CNS1/UniCNS-UTF16-H_0.cpp
           core/fpdfapi/cmaps/CNS1/cmaps_cns1.cpp
           core/fpdfapi/cmaps/GB1/Adobe-GB1-UCS2_5.cpp
           core/fpdfapi/cmaps/GB1/GB-EUC-H_0.cpp
           core/fpdfapi/cmaps/GB1/GB-EUC-V_0.cpp
           core/fpdfapi/cmaps/GB1/GBK-EUC-H_2.cpp
           core/fpdfapi/cmaps/GB1/GBK-EUC-V_2.cpp
           core/fpdfapi/cmaps/GB1/GBK2K-H_5.cpp
           core/fpdfapi/cmaps/GB1/GBK2K-V_5.cpp
           core/fpdfapi/cmaps/GB1/GBKp-EUC-H_2.cpp
           core/fpdfapi/cmaps/GB1/GBKp-EUC-V_2.cpp
           core/fpdfapi/cmaps/GB1/GBpc-EUC-H_0.cpp
           core/fpdfapi/cmaps/GB1/GBpc-EUC-V_0.cpp
           core/fpdfapi/cmaps/GB1/UniGB-UCS2-H_4.cpp
           core/fpdfapi/cmaps/GB1/UniGB-UCS2-V_4.cpp
           core/fpdfapi/cmaps/GB1/cmaps_gb1.cpp
           core/fpdfapi/cmaps/Japan1/83pv-RKSJ-H_1.cpp
           core/fpdfapi/cmaps/Japan1/90ms-RKSJ-H_2.cpp
           core/fpdfapi/cmaps/Japan1/90ms-RKSJ-V_2.cpp
           core/fpdfapi/cmaps/Japan1/90msp-RKSJ-H_2.cpp
           core/fpdfapi/cmaps/Japan1/90msp-RKSJ-V_2.cpp
           core/fpdfapi/cmaps/Japan1/90pv-RKSJ-H_1.cpp
           core/fpdfapi/cmaps/Japan1/Add-RKSJ-H_1.cpp
           core/fpdfapi/cmaps/Japan1/Add-RKSJ-V_1.cpp
           core/fpdfapi/cmaps/Japan1/Adobe-Japan1-UCS2_4.cpp
           core/fpdfapi/cmaps/Japan1/EUC-H_1.cpp
           core/fpdfapi/cmaps/Japan1/EUC-V_1.cpp
           core/fpdfapi/cmaps/Japan1/Ext-RKSJ-H_2.cpp
           core/fpdfapi/cmaps/Japan1/Ext-RKSJ-V_2.cpp
           core/fpdfapi/cmaps/Japan1/H_1.cpp
           core/fpdfapi/cmaps/Japan1/UniJIS-UCS2-HW-H_4.cpp
           core/fpdfapi/cmaps/Japan1/UniJIS-UCS2-HW-V_4.cpp
           core/fpdfapi/cmaps/Japan1/UniJIS-UCS2-H_4.cpp
           core/fpdfapi/cmaps/Japan1/UniJIS-UCS2-V_4.cpp
           core/fpdfapi/cmaps/Japan1/V_1.cpp
           core/fpdfapi/cmaps/Japan1/cmaps_japan1.cpp
           core/fpdfapi/cmaps/Korea1/Adobe-Korea1-UCS2_2.cpp
           core/fpdfapi/cmaps/Korea1/KSC-EUC-H_0.cpp
           core/fpdfapi/cmaps/Korea1/KSC-EUC-V_0.cpp
           core/fpdfapi/cmaps/Korea1/KSCms-UHC-HW-H_1.cpp
           core/fpdfapi/cmaps/Korea1/KSCms-UHC-HW-V_1.cpp
           core/fpdfapi/cmaps/Korea1/KSCms-UHC-H_1.cpp
           core/fpdfapi/cmaps/Korea1/KSCms-UHC-V_1.cpp
           core/fpdfapi/cmaps/Korea1/KSCpc-EUC-H_0.cpp
           core/fpdfapi/cmaps/Korea1/UniKS-UCS2-H_1.cpp
           core/fpdfapi/cmaps/Korea1/UniKS-UCS2-V_1.cpp
           core/fpdfapi/cmaps/Korea1/UniKS-UTF16-H_0.cpp
           core/fpdfapi/cmaps/Korea1/cmaps_korea1.cpp
           core/fpdfapi/cmaps/cmap_int.h
           core/fpdfapi/cmaps/fpdf_cmaps.cpp
)
SOURCE_GROUP ("fpdfapi\\cmaps" FILES ${fpdfapi_cmaps_files})

set (fpdfapi_edit_files
           core/fpdfapi/edit/cpdf_creator.cpp
           core/fpdfapi/edit/cpdf_creator.h
           core/fpdfapi/edit/cpdf_encryptor.cpp
           core/fpdfapi/edit/cpdf_encryptor.h
           core/fpdfapi/edit/cpdf_flateencoder.cpp
           core/fpdfapi/edit/cpdf_flateencoder.h
           core/fpdfapi/edit/cpdf_pagecontentgenerator.cpp
           core/fpdfapi/edit/cpdf_pagecontentgenerator.h
)
SOURCE_GROUP ("fpdfapi\\edit" FILES ${fpdfapi_edit_files})

set (fpdfapi_font_files
           core/fpdfapi/font/cfx_cttgsubtable.cpp
           core/fpdfapi/font/cfx_cttgsubtable.h
           core/fpdfapi/font/cfx_stockfontarray.cpp
           core/fpdfapi/font/cfx_stockfontarray.h
           core/fpdfapi/font/cpdf_cid2unicodemap.cpp
           core/fpdfapi/font/cpdf_cid2unicodemap.h
           core/fpdfapi/font/cpdf_cidfont.cpp
           core/fpdfapi/font/cpdf_cidfont.h
           core/fpdfapi/font/cpdf_cmap.cpp
           core/fpdfapi/font/cpdf_cmap.h
           core/fpdfapi/font/cpdf_cmapmanager.cpp
           core/fpdfapi/font/cpdf_cmapmanager.h
           core/fpdfapi/font/cpdf_cmapparser.cpp
           core/fpdfapi/font/cpdf_cmapparser.h
           core/fpdfapi/font/cpdf_font.cpp
           core/fpdfapi/font/cpdf_font.h
           core/fpdfapi/font/cpdf_fontencoding.cpp
           core/fpdfapi/font/cpdf_fontencoding.h
           core/fpdfapi/font/cpdf_fontglobals.cpp
           core/fpdfapi/font/cpdf_fontglobals.h
           core/fpdfapi/font/cpdf_simplefont.cpp
           core/fpdfapi/font/cpdf_simplefont.h
           core/fpdfapi/font/cpdf_tounicodemap.cpp
           core/fpdfapi/font/cpdf_tounicodemap.h
           core/fpdfapi/font/cpdf_truetypefont.cpp
           core/fpdfapi/font/cpdf_truetypefont.h
           core/fpdfapi/font/cpdf_type1font.cpp
           core/fpdfapi/font/cpdf_type1font.h
           core/fpdfapi/font/cpdf_type3char.cpp
           core/fpdfapi/font/cpdf_type3char.h
           core/fpdfapi/font/cpdf_type3font.cpp
           core/fpdfapi/font/cpdf_type3font.h
)
SOURCE_GROUP ("fpdfapi\\font" FILES ${fpdfapi_font_files})

set (fpdfapi_page_files
           core/fpdfapi/page/cpdf_allstates.cpp
           core/fpdfapi/page/cpdf_allstates.h
           core/fpdfapi/page/cpdf_clippath.cpp
           core/fpdfapi/page/cpdf_clippath.h
           core/fpdfapi/page/cpdf_color.cpp
           core/fpdfapi/page/cpdf_color.h
           core/fpdfapi/page/cpdf_colorspace.cpp
           core/fpdfapi/page/cpdf_colorspace.h
           core/fpdfapi/page/cpdf_colorstate.cpp
           core/fpdfapi/page/cpdf_colorstate.h
           core/fpdfapi/page/cpdf_contentmark.cpp
           core/fpdfapi/page/cpdf_contentmark.h
           core/fpdfapi/page/cpdf_contentmarkitem.cpp
           core/fpdfapi/page/cpdf_contentmarkitem.h
           core/fpdfapi/page/cpdf_contentparser.cpp
           core/fpdfapi/page/cpdf_contentparser.h
           core/fpdfapi/page/cpdf_countedobject.h
           core/fpdfapi/page/cpdf_devicecs.cpp
           core/fpdfapi/page/cpdf_devicecs.h
           core/fpdfapi/page/cpdf_docpagedata.cpp
           core/fpdfapi/page/cpdf_docpagedata.h
           core/fpdfapi/page/cpdf_expintfunc.cpp
           core/fpdfapi/page/cpdf_expintfunc.h
           core/fpdfapi/page/cpdf_form.cpp
           core/fpdfapi/page/cpdf_form.h
           core/fpdfapi/page/cpdf_formobject.cpp
           core/fpdfapi/page/cpdf_formobject.h
           core/fpdfapi/page/cpdf_function.cpp
           core/fpdfapi/page/cpdf_function.h
           core/fpdfapi/page/cpdf_generalstate.cpp
           core/fpdfapi/page/cpdf_generalstate.h
           core/fpdfapi/page/cpdf_graphicstates.cpp
           core/fpdfapi/page/cpdf_graphicstates.h
           core/fpdfapi/page/cpdf_iccprofile.cpp
           core/fpdfapi/page/cpdf_iccprofile.h
           core/fpdfapi/page/cpdf_image.cpp
           core/fpdfapi/page/cpdf_image.h
           core/fpdfapi/page/cpdf_imageobject.cpp
           core/fpdfapi/page/cpdf_imageobject.h
           core/fpdfapi/page/cpdf_meshstream.cpp
           core/fpdfapi/page/cpdf_meshstream.h
           core/fpdfapi/page/cpdf_page.cpp
           core/fpdfapi/page/cpdf_page.h
           core/fpdfapi/page/cpdf_pagemodule.cpp
           core/fpdfapi/page/cpdf_pagemodule.h
           core/fpdfapi/page/cpdf_pageobject.cpp
           core/fpdfapi/page/cpdf_pageobject.h
           core/fpdfapi/page/cpdf_pageobjectholder.cpp
           core/fpdfapi/page/cpdf_pageobjectholder.h
           core/fpdfapi/page/cpdf_pageobjectlist.cpp
           core/fpdfapi/page/cpdf_pageobjectlist.h
           core/fpdfapi/page/cpdf_path.cpp
           core/fpdfapi/page/cpdf_path.h
           core/fpdfapi/page/cpdf_pathobject.cpp
           core/fpdfapi/page/cpdf_pathobject.h
           core/fpdfapi/page/cpdf_pattern.cpp
           core/fpdfapi/page/cpdf_pattern.h
           core/fpdfapi/page/cpdf_patterncs.cpp
           core/fpdfapi/page/cpdf_patterncs.h
           core/fpdfapi/page/cpdf_psengine.cpp
           core/fpdfapi/page/cpdf_psengine.h
           core/fpdfapi/page/cpdf_psfunc.cpp
           core/fpdfapi/page/cpdf_psfunc.h
           core/fpdfapi/page/cpdf_sampledfunc.cpp
           core/fpdfapi/page/cpdf_sampledfunc.h
           core/fpdfapi/page/cpdf_shadingobject.cpp
           core/fpdfapi/page/cpdf_shadingobject.h
           core/fpdfapi/page/cpdf_shadingpattern.cpp
           core/fpdfapi/page/cpdf_shadingpattern.h
           core/fpdfapi/page/cpdf_stitchfunc.cpp
           core/fpdfapi/page/cpdf_stitchfunc.h
           core/fpdfapi/page/cpdf_streamcontentparser.cpp
           core/fpdfapi/page/cpdf_streamcontentparser.h
           core/fpdfapi/page/cpdf_streamparser.cpp
           core/fpdfapi/page/cpdf_streamparser.h
           core/fpdfapi/page/cpdf_textobject.cpp
           core/fpdfapi/page/cpdf_textobject.h
           core/fpdfapi/page/cpdf_textstate.cpp
           core/fpdfapi/page/cpdf_textstate.h
           core/fpdfapi/page/cpdf_tilingpattern.cpp
           core/fpdfapi/page/cpdf_tilingpattern.h
)
SOURCE_GROUP ("fpdfapi\\page" FILES ${fpdfapi_page_files})

set (fpdfapi_parser_files
           core/fpdfapi/parser/cfdf_document.cpp
           core/fpdfapi/parser/cfdf_document.h
           core/fpdfapi/parser/cpdf_array.cpp
           core/fpdfapi/parser/cpdf_array.h
           core/fpdfapi/parser/cpdf_boolean.cpp
           core/fpdfapi/parser/cpdf_boolean.h
           core/fpdfapi/parser/cpdf_cross_ref_avail.cpp
           core/fpdfapi/parser/cpdf_cross_ref_avail.h
           core/fpdfapi/parser/cpdf_crypto_handler.cpp
           core/fpdfapi/parser/cpdf_crypto_handler.h
           core/fpdfapi/parser/cpdf_data_avail.cpp
           core/fpdfapi/parser/cpdf_data_avail.h
           core/fpdfapi/parser/cpdf_dictionary.cpp
           core/fpdfapi/parser/cpdf_dictionary.h
           core/fpdfapi/parser/cpdf_document.cpp
           core/fpdfapi/parser/cpdf_document.h
           core/fpdfapi/parser/cpdf_hint_tables.cpp
           core/fpdfapi/parser/cpdf_hint_tables.h
           core/fpdfapi/parser/cpdf_indirect_object_holder.cpp
           core/fpdfapi/parser/cpdf_indirect_object_holder.h
           core/fpdfapi/parser/cpdf_linearized_header.cpp
           core/fpdfapi/parser/cpdf_linearized_header.h
           core/fpdfapi/parser/cpdf_name.cpp
           core/fpdfapi/parser/cpdf_name.h
           core/fpdfapi/parser/cpdf_null.cpp
           core/fpdfapi/parser/cpdf_null.h
           core/fpdfapi/parser/cpdf_number.cpp
           core/fpdfapi/parser/cpdf_number.h
           core/fpdfapi/parser/cpdf_object.cpp
           core/fpdfapi/parser/cpdf_object.h
           core/fpdfapi/parser/cpdf_object_avail.cpp
           core/fpdfapi/parser/cpdf_object_avail.h
           core/fpdfapi/parser/cpdf_object_walker.cpp
           core/fpdfapi/parser/cpdf_object_walker.h
           core/fpdfapi/parser/cpdf_page_object_avail.cpp
           core/fpdfapi/parser/cpdf_page_object_avail.h
           core/fpdfapi/parser/cpdf_parser.cpp
           core/fpdfapi/parser/cpdf_parser.h
           core/fpdfapi/parser/cpdf_read_validator.cpp
           core/fpdfapi/parser/cpdf_read_validator.h
           core/fpdfapi/parser/cpdf_reference.cpp
           core/fpdfapi/parser/cpdf_reference.h
           core/fpdfapi/parser/cpdf_security_handler.cpp
           core/fpdfapi/parser/cpdf_security_handler.h
           core/fpdfapi/parser/cpdf_simple_parser.cpp
           core/fpdfapi/parser/cpdf_simple_parser.h
           core/fpdfapi/parser/cpdf_stream.cpp
           core/fpdfapi/parser/cpdf_stream.h
           core/fpdfapi/parser/cpdf_stream_acc.cpp
           core/fpdfapi/parser/cpdf_stream_acc.h
           core/fpdfapi/parser/cpdf_string.cpp
           core/fpdfapi/parser/cpdf_string.h
           core/fpdfapi/parser/cpdf_syntax_parser.cpp
           core/fpdfapi/parser/cpdf_syntax_parser.h
           core/fpdfapi/parser/fpdf_parser_decode.cpp
           core/fpdfapi/parser/fpdf_parser_decode.h
           core/fpdfapi/parser/fpdf_parser_utility.cpp
           core/fpdfapi/parser/fpdf_parser_utility.h
)
SOURCE_GROUP ("fpdfapi\\parser" FILES ${fpdfapi_parser_files})

set (fpdfapi_render_files
           core/fpdfapi/render/cpdf_charposlist.cpp
           core/fpdfapi/render/cpdf_charposlist.h
           core/fpdfapi/render/cpdf_devicebuffer.cpp
           core/fpdfapi/render/cpdf_devicebuffer.h
           core/fpdfapi/render/cpdf_dibsource.cpp
           core/fpdfapi/render/cpdf_dibsource.h
           core/fpdfapi/render/cpdf_dibtransferfunc.cpp
           core/fpdfapi/render/cpdf_dibtransferfunc.h
           core/fpdfapi/render/cpdf_docrenderdata.cpp
           core/fpdfapi/render/cpdf_docrenderdata.h
           core/fpdfapi/render/cpdf_imagecacheentry.cpp
           core/fpdfapi/render/cpdf_imagecacheentry.h
           core/fpdfapi/render/cpdf_imageloader.cpp
           core/fpdfapi/render/cpdf_imageloader.h
           core/fpdfapi/render/cpdf_imagerenderer.cpp
           core/fpdfapi/render/cpdf_imagerenderer.h
           core/fpdfapi/render/cpdf_pagerendercache.cpp
           core/fpdfapi/render/cpdf_pagerendercache.h
           core/fpdfapi/render/cpdf_progressiverenderer.cpp
           core/fpdfapi/render/cpdf_progressiverenderer.h
           core/fpdfapi/render/cpdf_rendercontext.cpp
           core/fpdfapi/render/cpdf_rendercontext.h
           core/fpdfapi/render/cpdf_renderoptions.cpp
           core/fpdfapi/render/cpdf_renderoptions.h
           core/fpdfapi/render/cpdf_renderstatus.cpp
           core/fpdfapi/render/cpdf_renderstatus.h
           core/fpdfapi/render/cpdf_scaledrenderbuffer.cpp
           core/fpdfapi/render/cpdf_scaledrenderbuffer.h
           core/fpdfapi/render/cpdf_textrenderer.cpp
           core/fpdfapi/render/cpdf_textrenderer.h
           core/fpdfapi/render/cpdf_transferfunc.cpp
           core/fpdfapi/render/cpdf_transferfunc.h
           core/fpdfapi/render/cpdf_type3cache.cpp
           core/fpdfapi/render/cpdf_type3cache.h
           core/fpdfapi/render/cpdf_type3glyphs.cpp
           core/fpdfapi/render/cpdf_type3glyphs.h
)
SOURCE_GROUP ("fpdfapi\\render" FILES ${fpdfapi_render_files})

set (fpdfapi_files
           core/fpdfapi/cpdf_modulemgr.cpp
           core/fpdfapi/cpdf_modulemgr.h
           core/fpdfapi/cpdf_pagerendercontext.cpp
           core/fpdfapi/cpdf_pagerendercontext.h
)
SOURCE_GROUP ("fpdfapi" FILES ${fpdfapi_files})

set (fxjs_files
           fxjs/ijs_event_context.h
           fxjs/ijs_runtime.h
           fxjs/cjs_event_context_stub.cpp
           fxjs/cjs_event_context_stub.h
           fxjs/cjs_runtimestub.cpp
)
SOURCE_GROUP ("fxjs" FILES ${fxjs_files})

set (fdrm_files
           core/fdrm/crypto/fx_crypt.cpp
           core/fdrm/crypto/fx_crypt.h
           core/fdrm/crypto/fx_crypt_aes.cpp
           core/fdrm/crypto/fx_crypt_sha.cpp
)
SOURCE_GROUP ("fdrm" FILES ${fdrm_files})

set (fxcodec_files
           core/fxcodec/JBig2_DocumentContext.h
           core/fxcodec/fx_codec.h
           core/fxcodec/fx_codec_def.h
)
SOURCE_GROUP ("fxcodec" FILES ${fxcodec_files})

set (fxcodec_codec_files
           core/fxcodec/codec/ccodec_basicmodule.h
           core/fxcodec/codec/ccodec_faxmodule.h
           core/fxcodec/codec/ccodec_flatemodule.h
           core/fxcodec/codec/ccodec_iccmodule.h
           core/fxcodec/codec/ccodec_jbig2module.h
           core/fxcodec/codec/ccodec_jpegmodule.h
           core/fxcodec/codec/ccodec_jpxmodule.h
           core/fxcodec/codec/ccodec_scanlinedecoder.cpp
           core/fxcodec/codec/ccodec_scanlinedecoder.h
           core/fxcodec/codec/cjpx_decoder.h
           core/fxcodec/codec/codec_int.h
           core/fxcodec/codec/fx_codec.cpp
           core/fxcodec/codec/fx_codec_fax.cpp
           core/fxcodec/codec/fx_codec_flate.cpp
           core/fxcodec/codec/fx_codec_icc.cpp
           core/fxcodec/codec/fx_codec_jbig.cpp
           core/fxcodec/codec/fx_codec_jpeg.cpp
           core/fxcodec/codec/fx_codec_jpx_opj.cpp
)
SOURCE_GROUP ("fxcodec\\codec" FILES ${fxcodec_codec_files})

set (fxcodec_jbig2_files
           core/fxcodec/jbig2/JBig2_ArithDecoder.cpp
           core/fxcodec/jbig2/JBig2_ArithDecoder.h
           core/fxcodec/jbig2/JBig2_ArithIntDecoder.cpp
           core/fxcodec/jbig2/JBig2_ArithIntDecoder.h
           core/fxcodec/jbig2/JBig2_BitStream.cpp
           core/fxcodec/jbig2/JBig2_BitStream.h
           core/fxcodec/jbig2/JBig2_Context.cpp
           core/fxcodec/jbig2/JBig2_Context.h
           core/fxcodec/jbig2/JBig2_Define.h
           core/fxcodec/jbig2/JBig2_GrdProc.cpp
           core/fxcodec/jbig2/JBig2_GrdProc.h
           core/fxcodec/jbig2/JBig2_GrrdProc.cpp
           core/fxcodec/jbig2/JBig2_GrrdProc.h
           core/fxcodec/jbig2/JBig2_HtrdProc.cpp
           core/fxcodec/jbig2/JBig2_HtrdProc.h
           core/fxcodec/jbig2/JBig2_HuffmanDecoder.cpp
           core/fxcodec/jbig2/JBig2_HuffmanDecoder.h
           core/fxcodec/jbig2/JBig2_HuffmanTable.cpp
           core/fxcodec/jbig2/JBig2_HuffmanTable.h
           core/fxcodec/jbig2/JBig2_HuffmanTable_Standard.cpp
           core/fxcodec/jbig2/JBig2_HuffmanTable_Standard.h
           core/fxcodec/jbig2/JBig2_Image.cpp
           core/fxcodec/jbig2/JBig2_Image.h
           core/fxcodec/jbig2/JBig2_Page.h
           core/fxcodec/jbig2/JBig2_PatternDict.cpp
           core/fxcodec/jbig2/JBig2_PatternDict.h
           core/fxcodec/jbig2/JBig2_PddProc.cpp
           core/fxcodec/jbig2/JBig2_PddProc.h
           core/fxcodec/jbig2/JBig2_SddProc.cpp
           core/fxcodec/jbig2/JBig2_SddProc.h
           core/fxcodec/jbig2/JBig2_Segment.cpp
           core/fxcodec/jbig2/JBig2_Segment.h
           core/fxcodec/jbig2/JBig2_SymbolDict.cpp
           core/fxcodec/jbig2/JBig2_SymbolDict.h
           core/fxcodec/jbig2/JBig2_TrdProc.cpp
           core/fxcodec/jbig2/JBig2_TrdProc.h
)
SOURCE_GROUP ("fxcodec\\jbig2" FILES ${fxcodec_jbig2_files})

set (fpdftext_files
           core/fpdftext/cpdf_linkextract.cpp
           core/fpdftext/cpdf_linkextract.h
           core/fpdftext/cpdf_textpage.cpp
           core/fpdftext/cpdf_textpage.h
           core/fpdftext/cpdf_textpagefind.cpp
           core/fpdftext/cpdf_textpagefind.h
           core/fpdftext/unicodenormalizationdata.cpp
           core/fpdftext/unicodenormalizationdata.h
)
SOURCE_GROUP ("fpdftext" FILES ${fpdftext_files})

set (fpdfdoc_files
           core/fpdfdoc/cline.cpp
           core/fpdfdoc/cline.h
           core/fpdfdoc/cpdf_aaction.cpp
           core/fpdfdoc/cpdf_aaction.h
           core/fpdfdoc/cpdf_action.cpp
           core/fpdfdoc/cpdf_action.h
           core/fpdfdoc/cpdf_actionfields.cpp
           core/fpdfdoc/cpdf_actionfields.h
           core/fpdfdoc/cpdf_annot.cpp
           core/fpdfdoc/cpdf_annot.h
           core/fpdfdoc/cpdf_annotlist.cpp
           core/fpdfdoc/cpdf_annotlist.h
           core/fpdfdoc/cpdf_apsettings.cpp
           core/fpdfdoc/cpdf_apsettings.h
           core/fpdfdoc/cpdf_bookmark.cpp
           core/fpdfdoc/cpdf_bookmark.h
           core/fpdfdoc/cpdf_bookmarktree.cpp
           core/fpdfdoc/cpdf_bookmarktree.h
           core/fpdfdoc/cpdf_defaultappearance.cpp
           core/fpdfdoc/cpdf_defaultappearance.h
           core/fpdfdoc/cpdf_dest.cpp
           core/fpdfdoc/cpdf_dest.h
           core/fpdfdoc/cpdf_docjsactions.cpp
           core/fpdfdoc/cpdf_docjsactions.h
           core/fpdfdoc/cpdf_filespec.cpp
           core/fpdfdoc/cpdf_filespec.h
           core/fpdfdoc/cpdf_formcontrol.cpp
           core/fpdfdoc/cpdf_formcontrol.h
           core/fpdfdoc/cpdf_formfield.cpp
           core/fpdfdoc/cpdf_formfield.h
           core/fpdfdoc/cpdf_iconfit.cpp
           core/fpdfdoc/cpdf_iconfit.h
           core/fpdfdoc/cpdf_interform.cpp
           core/fpdfdoc/cpdf_interform.h
           core/fpdfdoc/cpdf_link.cpp
           core/fpdfdoc/cpdf_link.h
           core/fpdfdoc/cpdf_linklist.cpp
           core/fpdfdoc/cpdf_linklist.h
           core/fpdfdoc/cpdf_metadata.cpp
           core/fpdfdoc/cpdf_metadata.h
           core/fpdfdoc/cpdf_nametree.cpp
           core/fpdfdoc/cpdf_nametree.h
           core/fpdfdoc/cpdf_numbertree.cpp
           core/fpdfdoc/cpdf_numbertree.h
           core/fpdfdoc/cpdf_occontext.cpp
           core/fpdfdoc/cpdf_occontext.h
           core/fpdfdoc/cpdf_pagelabel.cpp
           core/fpdfdoc/cpdf_pagelabel.h
           core/fpdfdoc/cpdf_structelement.cpp
           core/fpdfdoc/cpdf_structelement.h
           core/fpdfdoc/cpdf_structtree.cpp
           core/fpdfdoc/cpdf_structtree.h
           core/fpdfdoc/cpdf_variabletext.cpp
           core/fpdfdoc/cpdf_variabletext.h
           core/fpdfdoc/cpdf_viewerpreferences.cpp
           core/fpdfdoc/cpdf_viewerpreferences.h
           core/fpdfdoc/cpvt_floatrect.h
           core/fpdfdoc/cpvt_fontmap.cpp
           core/fpdfdoc/cpvt_fontmap.h
           core/fpdfdoc/cpvt_generateap.cpp
           core/fpdfdoc/cpvt_generateap.h
           core/fpdfdoc/cpvt_line.h
           core/fpdfdoc/cpvt_lineinfo.h
           core/fpdfdoc/cpvt_word.h
           core/fpdfdoc/cpvt_wordinfo.cpp
           core/fpdfdoc/cpvt_wordinfo.h
           core/fpdfdoc/cpvt_wordplace.h
           core/fpdfdoc/cpvt_wordrange.h
           core/fpdfdoc/csection.cpp
           core/fpdfdoc/csection.h
           core/fpdfdoc/ctypeset.cpp
           core/fpdfdoc/ctypeset.h
           core/fpdfdoc/ipdf_formnotify.h
           core/fpdfdoc/ipvt_fontmap.h
)
SOURCE_GROUP ("fpdfdoc" FILES ${fpdfdoc_files})

set (pdfium_base_files
           third_party/base/base_export.h
           third_party/base/bits.h
           third_party/base/compiler_specific.h
           third_party/base/logging.h
           third_party/base/macros.h
           third_party/base/optional.h
           third_party/base/ptr_util.h
           third_party/base/stl_util.h
           third_party/base/sys_byteorder.h
           third_party/base/template_util.h
           third_party/build/build_config.h
)
SOURCE_GROUP ("pdfium_base" FILES ${pdfium_base_files})

set (pdfium_base_allocator_files
           third_party/base/allocator/partition_allocator/address_space_randomization.cc
           third_party/base/allocator/partition_allocator/address_space_randomization.h
           third_party/base/allocator/partition_allocator/oom.h
           third_party/base/allocator/partition_allocator/page_allocator.cc
           third_party/base/allocator/partition_allocator/page_allocator.h
           third_party/base/allocator/partition_allocator/partition_alloc.cc
           third_party/base/allocator/partition_allocator/partition_alloc.h
           third_party/base/allocator/partition_allocator/spin_lock.cc
           third_party/base/allocator/partition_allocator/spin_lock.h
)
SOURCE_GROUP ("pdfium_base\\allocator" FILES ${pdfium_base_allocator_files})

set (pdfium_base_numeric_files
           third_party/base/numerics/safe_conversions.h
           third_party/base/numerics/safe_conversions_impl.h
           third_party/base/numerics/safe_math.h
           third_party/base/numerics/safe_math_impl.h
)
SOURCE_GROUP ("pdfium_base\\numeric" FILES ${pdfium_base_numeric_files})

set (fx_agg_files
           third_party/agg23/agg_basics.h
           third_party/agg23/agg_clip_liang_barsky.h
           third_party/agg23/agg_conv_dash.h
           third_party/agg23/agg_conv_stroke.h
           third_party/agg23/agg_curves.cpp
           third_party/agg23/agg_curves.h
           third_party/agg23/agg_path_storage.cpp
           third_party/agg23/agg_path_storage.h
           third_party/agg23/agg_rasterizer_scanline_aa.cpp
           third_party/agg23/agg_rasterizer_scanline_aa.h
           third_party/agg23/agg_renderer_scanline.h
           third_party/agg23/agg_rendering_buffer.h
           third_party/agg23/agg_scanline_u.h
           third_party/agg23/agg_vcgen_dash.cpp
           third_party/agg23/agg_vcgen_stroke.cpp
)
SOURCE_GROUP ("fx_agg" FILES ${fx_agg_files})

set (fxcrt_files
           core/fxcrt/autorestorer.h 
           core/fxcrt/bytestring.cpp 
           core/fxcrt/bytestring.h 
           core/fxcrt/cfx_binarybuf.cpp 
           core/fxcrt/cfx_binarybuf.h 
           core/fxcrt/cfx_bitstream.cpp 
           core/fxcrt/cfx_bitstream.h 
           core/fxcrt/cfx_datetime.cpp 
           core/fxcrt/cfx_datetime.h 
           core/fxcrt/cfx_fileaccess_posix.cpp 
           core/fxcrt/cfx_fileaccess_posix.h 
           core/fxcrt/cfx_fileaccess_windows.cpp 
           core/fxcrt/cfx_fileaccess_windows.h 
           core/fxcrt/cfx_fixedbufgrow.h 
           core/fxcrt/cfx_memorystream.cpp 
           core/fxcrt/cfx_memorystream.h 
           core/fxcrt/cfx_seekablemultistream.cpp 
           core/fxcrt/cfx_seekablemultistream.h 
           core/fxcrt/cfx_utf8decoder.cpp 
           core/fxcrt/cfx_utf8decoder.h 
           core/fxcrt/cfx_widetextbuf.cpp 
           core/fxcrt/cfx_widetextbuf.h 
           core/fxcrt/fx_bidi.cpp 
           core/fxcrt/fx_bidi.h 
           core/fxcrt/fx_codepage.h 
           core/fxcrt/fx_coordinates.cpp 
           core/fxcrt/fx_coordinates.h 
           core/fxcrt/fx_extension.cpp 
           core/fxcrt/fx_extension.h 
           core/fxcrt/fx_memory.cpp 
           core/fxcrt/fx_memory.h 
           core/fxcrt/fx_random.cpp 
           core/fxcrt/fx_random.h 
           core/fxcrt/fx_safe_types.h 
           core/fxcrt/fx_stream.cpp 
           core/fxcrt/fx_stream.h 
           core/fxcrt/fx_string.cpp 
           core/fxcrt/fx_string.h 
           core/fxcrt/fx_system.cpp 
           core/fxcrt/fx_system.h 
           core/fxcrt/fx_ucddata.cpp 
           core/fxcrt/fx_ucddata.h 
           core/fxcrt/fx_unicode.cpp 
           core/fxcrt/fx_unicode.h 
           core/fxcrt/ifx_fileaccess.h 
           core/fxcrt/ifx_pauseindicator.h 
           core/fxcrt/maybe_owned.h 
           core/fxcrt/observable.h 
           core/fxcrt/retain_ptr.h 
           core/fxcrt/shared_copy_on_write.h 
           core/fxcrt/string_data_template.h 
           core/fxcrt/string_pool_template.h 
           core/fxcrt/string_view_template.h 
           core/fxcrt/unowned_ptr.h 
           core/fxcrt/weak_ptr.h 
           core/fxcrt/widestring.cpp 
           core/fxcrt/widestring.h
)
SOURCE_GROUP ("fxcrt" FILES ${fxcrt_files})

set (fxcrt_xml_files
           core/fxcrt/xml/cxml_attritem.cpp 
           core/fxcrt/xml/cxml_attritem.h 
           core/fxcrt/xml/cxml_attrmap.cpp 
           core/fxcrt/xml/cxml_attrmap.h 
           core/fxcrt/xml/cxml_content.cpp 
           core/fxcrt/xml/cxml_content.h 
           core/fxcrt/xml/cxml_databufacc.cpp 
           core/fxcrt/xml/cxml_databufacc.h 
           core/fxcrt/xml/cxml_element.cpp 
           core/fxcrt/xml/cxml_element.h 
           core/fxcrt/xml/cxml_object.cpp 
           core/fxcrt/xml/cxml_object.h 
           core/fxcrt/xml/cxml_parser.cpp 
           core/fxcrt/xml/cxml_parser.h
)
SOURCE_GROUP ("fxcrt\\xml" FILES ${fxcrt_xml_files})

set (fx_lcms_files
           third_party/lcms/include/lcms2.h
           third_party/lcms/include/lcms2_plugin.h
           third_party/lcms/src/cmsalpha.cpp
           third_party/lcms/src/cmscam02.cpp
           third_party/lcms/src/cmscgats.cpp
           third_party/lcms/src/cmscnvrt.cpp
           third_party/lcms/src/cmserr.cpp
           third_party/lcms/src/cmsgamma.cpp
           third_party/lcms/src/cmsgmt.cpp
           third_party/lcms/src/cmshalf.cpp
           third_party/lcms/src/cmsintrp.cpp
           third_party/lcms/src/cmsio0.cpp
           third_party/lcms/src/cmsio1.cpp
           third_party/lcms/src/cmslut.cpp
           third_party/lcms/src/cmsmd5.cpp
           third_party/lcms/src/cmsmtrx.cpp
           third_party/lcms/src/cmsnamed.cpp
           third_party/lcms/src/cmsopt.cpp
           third_party/lcms/src/cmspack.cpp
           third_party/lcms/src/cmspcs.cpp
           third_party/lcms/src/cmsplugin.cpp
           third_party/lcms/src/cmsps2.cpp
           third_party/lcms/src/cmssamp.cpp
           third_party/lcms/src/cmssm.cpp
           third_party/lcms/src/cmstypes.cpp
           third_party/lcms/src/cmsvirt.cpp
           third_party/lcms/src/cmswtpnt.cpp
           third_party/lcms/src/cmsxform.cpp
)
SOURCE_GROUP ("fx_lcms2" FILES ${fx_lcms_files})

set (formfiller_files
           fpdfsdk/formfiller/cba_fontmap.cpp
           fpdfsdk/formfiller/cba_fontmap.h
           fpdfsdk/formfiller/cffl_button.cpp
           fpdfsdk/formfiller/cffl_button.h
           fpdfsdk/formfiller/cffl_checkbox.cpp
           fpdfsdk/formfiller/cffl_checkbox.h
           fpdfsdk/formfiller/cffl_combobox.cpp
           fpdfsdk/formfiller/cffl_combobox.h
           fpdfsdk/formfiller/cffl_formfiller.cpp
           fpdfsdk/formfiller/cffl_formfiller.h
           fpdfsdk/formfiller/cffl_interactiveformfiller.cpp
           fpdfsdk/formfiller/cffl_interactiveformfiller.h
           fpdfsdk/formfiller/cffl_listbox.cpp
           fpdfsdk/formfiller/cffl_listbox.h
           fpdfsdk/formfiller/cffl_pushbutton.cpp
           fpdfsdk/formfiller/cffl_pushbutton.h
           fpdfsdk/formfiller/cffl_radiobutton.cpp
           fpdfsdk/formfiller/cffl_radiobutton.h
           fpdfsdk/formfiller/cffl_textfield.cpp
           fpdfsdk/formfiller/cffl_textfield.h
           fpdfsdk/formfiller/cffl_textobject.cpp
           fpdfsdk/formfiller/cffl_textobject.h
)
SOURCE_GROUP ("formfiller" FILES ${formfiller_files})

set (pwl_files
           fpdfsdk/pwl/cpwl_appstream.cpp
           fpdfsdk/pwl/cpwl_appstream.h
           fpdfsdk/pwl/cpwl_button.cpp
           fpdfsdk/pwl/cpwl_button.h
           fpdfsdk/pwl/cpwl_caret.cpp
           fpdfsdk/pwl/cpwl_caret.h
           fpdfsdk/pwl/cpwl_combo_box.cpp
           fpdfsdk/pwl/cpwl_combo_box.h
           fpdfsdk/pwl/cpwl_edit.cpp
           fpdfsdk/pwl/cpwl_edit.h
           fpdfsdk/pwl/cpwl_edit_ctrl.cpp
           fpdfsdk/pwl/cpwl_edit_ctrl.h
           fpdfsdk/pwl/cpwl_edit_impl.cpp
           fpdfsdk/pwl/cpwl_edit_impl.h
           fpdfsdk/pwl/cpwl_font_map.cpp
           fpdfsdk/pwl/cpwl_font_map.h
           fpdfsdk/pwl/cpwl_icon.cpp
           fpdfsdk/pwl/cpwl_icon.h
           fpdfsdk/pwl/cpwl_list_box.cpp
           fpdfsdk/pwl/cpwl_list_box.h
           fpdfsdk/pwl/cpwl_list_impl.cpp
           fpdfsdk/pwl/cpwl_list_impl.h
           fpdfsdk/pwl/cpwl_scroll_bar.cpp
           fpdfsdk/pwl/cpwl_scroll_bar.h
           fpdfsdk/pwl/cpwl_special_button.cpp
           fpdfsdk/pwl/cpwl_special_button.h
           fpdfsdk/pwl/cpwl_timer.cpp
           fpdfsdk/pwl/cpwl_timer.h
           fpdfsdk/pwl/cpwl_timer_handler.cpp
           fpdfsdk/pwl/cpwl_timer_handler.h
           fpdfsdk/pwl/cpwl_wnd.cpp
           fpdfsdk/pwl/cpwl_wnd.h
)
SOURCE_GROUP ("pwl" FILES ${pwl_files})

set (fxge_files
           core/fxge/cfx_cliprgn.cpp
           core/fxge/cfx_cliprgn.h
           core/fxge/cfx_color.cpp
           core/fxge/cfx_color.h
           core/fxge/cfx_defaultrenderdevice.h
           core/fxge/cfx_facecache.cpp
           core/fxge/cfx_facecache.h
           core/fxge/cfx_folderfontinfo.cpp
           core/fxge/cfx_folderfontinfo.h
           core/fxge/cfx_font.cpp
           core/fxge/cfx_fontcache.cpp
           core/fxge/cfx_fontcache.h
           core/fxge/cfx_fontmapper.cpp
           core/fxge/cfx_fontmapper.h
           core/fxge/cfx_fontmgr.cpp
           core/fxge/cfx_fontmgr.h
           core/fxge/cfx_gemodule.cpp
           core/fxge/cfx_gemodule.h
           core/fxge/cfx_graphstate.cpp
           core/fxge/cfx_graphstate.h
           core/fxge/cfx_graphstatedata.cpp
           core/fxge/cfx_graphstatedata.h
           core/fxge/cfx_pathdata.cpp
           core/fxge/cfx_pathdata.h
           core/fxge/cfx_renderdevice.cpp
           core/fxge/cfx_renderdevice.h
           core/fxge/cfx_substfont.cpp
           core/fxge/cfx_substfont.h
           core/fxge/cfx_unicodeencoding.cpp
           core/fxge/cfx_unicodeencoding.h
           core/fxge/cfx_windowsrenderdevice.h
           core/fxge/cttfontdesc.cpp
           core/fxge/cttfontdesc.h
           core/fxge/fx_dib.h
           core/fxge/fx_font.h
           core/fxge/fx_freetype.h
           core/fxge/fx_ge_fontmap.cpp
           core/fxge/fx_ge_linux.cpp
           core/fxge/fx_ge_text.cpp
           core/fxge/ifx_renderdevicedriver.cpp
           core/fxge/ifx_renderdevicedriver.h
           core/fxge/ifx_systemfontinfo.h
)
SOURCE_GROUP ("fxge" FILES ${fxge_files})

set (fxge_freetype_files
           core/fxge/freetype/fx_freetype.cpp
)
SOURCE_GROUP ("fxge\\freetype" FILES ${fxge_freetype_files})

set (fxge_fontdata_files
           core/fxge/fontdata/chromefontdata/FoxitDingbats.cpp
           core/fxge/fontdata/chromefontdata/FoxitFixed.cpp
           core/fxge/fontdata/chromefontdata/FoxitFixedBold.cpp
           core/fxge/fontdata/chromefontdata/FoxitFixedBoldItalic.cpp
           core/fxge/fontdata/chromefontdata/FoxitFixedItalic.cpp
           core/fxge/fontdata/chromefontdata/FoxitSans.cpp
           core/fxge/fontdata/chromefontdata/FoxitSansBold.cpp
           core/fxge/fontdata/chromefontdata/FoxitSansBoldItalic.cpp
           core/fxge/fontdata/chromefontdata/FoxitSansItalic.cpp
           core/fxge/fontdata/chromefontdata/FoxitSansMM.cpp
           core/fxge/fontdata/chromefontdata/FoxitSerif.cpp
           core/fxge/fontdata/chromefontdata/FoxitSerifBold.cpp
           core/fxge/fontdata/chromefontdata/FoxitSerifBoldItalic.cpp
           core/fxge/fontdata/chromefontdata/FoxitSerifItalic.cpp
           core/fxge/fontdata/chromefontdata/FoxitSerifMM.cpp
           core/fxge/fontdata/chromefontdata/FoxitSymbol.cpp
           core/fxge/fontdata/chromefontdata/chromefontdata.h
)
SOURCE_GROUP ("fxge\\fontdata" FILES ${fxge_fontdata_files})

set (fxge_dib_files
           core/fxge/dib/cfx_bitmapcomposer.cpp
           core/fxge/dib/cfx_bitmapcomposer.h
           core/fxge/dib/cfx_bitmapstorer.cpp
           core/fxge/dib/cfx_bitmapstorer.h
           core/fxge/dib/cfx_dibextractor.cpp
           core/fxge/dib/cfx_dibextractor.h
           core/fxge/dib/cfx_dibitmap.cpp
           core/fxge/dib/cfx_dibitmap.h
           core/fxge/dib/cfx_dibsource.cpp
           core/fxge/dib/cfx_dibsource.h
           core/fxge/dib/cfx_filtereddib.cpp
           core/fxge/dib/cfx_filtereddib.h
           core/fxge/dib/cfx_imagerenderer.cpp
           core/fxge/dib/cfx_imagerenderer.h
           core/fxge/dib/cfx_imagestretcher.cpp
           core/fxge/dib/cfx_imagestretcher.h
           core/fxge/dib/cfx_imagetransformer.cpp
           core/fxge/dib/cfx_imagetransformer.h
           core/fxge/dib/cfx_scanlinecompositor.cpp
           core/fxge/dib/cfx_scanlinecompositor.h
           core/fxge/dib/cstretchengine.cpp
           core/fxge/dib/cstretchengine.h
           core/fxge/dib/fx_dib_main.cpp
           core/fxge/dib/ifx_scanlinecomposer.h
)
SOURCE_GROUP ("fxge\\dib" FILES ${fxge_dib_files})

set (fxge_agg_files
           core/fxge/agg/fx_agg_driver.cpp
           core/fxge/agg/fx_agg_driver.h
)

SOURCE_GROUP ("fxge\\agg" FILES ${fxge_agg_files})

if (APPLE)
set (fxge_macos_files
           core/fxge/apple/fx_apple_platform.cpp
           core/fxge/apple/apple_int.h
           core/fxge/apple/fx_mac_imp.cpp
           core/fxge/apple/fx_quartz_device.cpp
)
endif()
SOURCE_GROUP ("fxge\\macos" FILES ${fxge_macos_files})

if (WIN32)
set (fxge_win32_files
           core/fxge/win32/cfx_windowsdib.h
           core/fxge/win32/cfx_psrenderer.cpp
           core/fxge/win32/cfx_psrenderer.h
           core/fxge/win32/cpsoutput.cpp
           core/fxge/win32/cpsoutput.h
           core/fxge/win32/dwrite_int.h
           core/fxge/win32/fx_win32_device.cpp
           core/fxge/win32/fx_win32_dib.cpp
           core/fxge/win32/fx_win32_dwrite.cpp
           core/fxge/win32/fx_win32_gdipext.cpp
           core/fxge/win32/fx_win32_print.cpp
           core/fxge/win32/win32_int.h

)
else ()
set (fxge_win32_files
           core/fxge/win32/cfx_windowsdib.h
)
endif()
SOURCE_GROUP ("fxge\\win32" FILES ${fxge_win32_files})

if (ANDROID)
set (fxge_android_files
           core/fxge/android/cfpf_skiabufferfont.cpp
           core/fxge/android/cfpf_skiabufferfont.h
           core/fxge/android/cfpf_skiadevicemodule.cpp
           core/fxge/android/cfpf_skiadevicemodule.h
           core/fxge/android/cfpf_skiafilefont.cpp
           core/fxge/android/cfpf_skiafilefont.h
           core/fxge/android/cfpf_skiafont.cpp
           core/fxge/android/cfpf_skiafont.h
           core/fxge/android/cfpf_skiafontdescriptor.cpp
           core/fxge/android/cfpf_skiafontdescriptor.h
           core/fxge/android/cfpf_skiafontmgr.cpp
           core/fxge/android/cfpf_skiafontmgr.h
           core/fxge/android/cfpf_skiapathfont.cpp
           core/fxge/android/cfpf_skiapathfont.h
           core/fxge/android/cfx_androidfontinfo.cpp
           core/fxge/android/cfx_androidfontinfo.h
           core/fxge/android/fx_android_imp.cpp
)
endif()
SOURCE_GROUP ("fxge\\android" FILES ${fxge_android_files})

set (fpdfsdk_files
           fpdfsdk/cba_annotiterator.cpp
           fpdfsdk/cba_annotiterator.h
           fpdfsdk/cfx_systemhandler.cpp
           fpdfsdk/cfx_systemhandler.h
           fpdfsdk/cpdfsdk_annot.cpp
           fpdfsdk/cpdfsdk_annot.h
           fpdfsdk/cpdfsdk_annothandlermgr.cpp
           fpdfsdk/cpdfsdk_annothandlermgr.h
           fpdfsdk/cpdfsdk_annotiteration.cpp
           fpdfsdk/cpdfsdk_annotiteration.h
           fpdfsdk/cpdfsdk_baannot.cpp
           fpdfsdk/cpdfsdk_baannot.h
           fpdfsdk/cpdfsdk_baannothandler.cpp
           fpdfsdk/cpdfsdk_baannothandler.h
           fpdfsdk/cpdfsdk_datetime.cpp
           fpdfsdk/cpdfsdk_datetime.h
           fpdfsdk/cpdfsdk_formfillenvironment.cpp
           fpdfsdk/cpdfsdk_formfillenvironment.h
           fpdfsdk/cpdfsdk_interform.cpp
           fpdfsdk/cpdfsdk_interform.h
           fpdfsdk/cpdfsdk_pageview.cpp
           fpdfsdk/cpdfsdk_pageview.h
           fpdfsdk/cpdfsdk_widget.cpp
           fpdfsdk/cpdfsdk_widget.h
           fpdfsdk/cpdfsdk_widgethandler.cpp
           fpdfsdk/cpdfsdk_widgethandler.h
           fpdfsdk/fpdf_dataavail.cpp
           fpdfsdk/fpdf_ext.cpp
           fpdfsdk/fpdf_flatten.cpp
           fpdfsdk/fpdf_progressive.cpp
           fpdfsdk/fpdf_searchex.cpp
           fpdfsdk/fpdf_structtree.cpp
           fpdfsdk/fpdf_sysfontinfo.cpp
           fpdfsdk/fpdf_transformpage.cpp
           fpdfsdk/fpdfannot.cpp
           fpdfsdk/fpdfattachment.cpp
           fpdfsdk/fpdfcatalog.cpp
           fpdfsdk/fpdfdoc.cpp
           fpdfsdk/fpdfeditimg.cpp
           fpdfsdk/fpdfeditpage.cpp
           fpdfsdk/fpdfeditpath.cpp
           fpdfsdk/fpdfedittext.cpp
           fpdfsdk/fpdfformfill.cpp
           fpdfsdk/fpdfppo.cpp
           fpdfsdk/fpdfsave.cpp
           fpdfsdk/fpdftext.cpp
           fpdfsdk/fpdfview.cpp
           fpdfsdk/fsdk_actionhandler.cpp
           fpdfsdk/fsdk_actionhandler.h
           fpdfsdk/fsdk_filewriteadapter.cpp
           fpdfsdk/fsdk_filewriteadapter.h
           fpdfsdk/fsdk_pauseadapter.cpp
           fpdfsdk/fsdk_pauseadapter.h
           fpdfsdk/pdfsdk_fieldaction.cpp
           fpdfsdk/pdfsdk_fieldaction.h
)
SOURCE_GROUP ("fpdfsdk" FILES ${fpdfsdk_files})

th_sources(${TH_PDFIUM_LIB}
           ${fdrm_files}
           ${formfiller_files}
           ${fpdfapi_files}
           ${fpdfapi_cmaps_files}
           ${fpdfapi_edit_files}
           ${fpdfapi_font_files}
           ${fpdfapi_page_files}
           ${fpdfapi_parser_files}
           ${fpdfapi_render_files}
           ${fpdfdoc_files}
           ${fpdfsdk_files}
           ${fpdftext_files}
           ${fx_agg_files}
           ${fxge_files}
           ${fxge_android_files}
           ${fxge_win32_files}
           ${fxge_macos_files}
           ${fxge_dib_files}
           ${fxge_fontdata_files}
           ${fxge_freetype_files}
           ${fxge_agg_files}
           ${fxjs_files}
           ${fxcodec_files}
           ${fxcodec_codec_files}
           ${fxcodec_jbig2_files}
           ${fxcrt_files}
           ${fxcrt_xml_files}
           ${fx_lcms_files}
           ${pdfium_base_files}
           ${pdfium_base_allocator_files}
           ${pdfium_base_numeric_files}
           ${pwl_files}
           ${public_files})

include_directories(${TH_ROOT}/pdfium
                    ${TH_ROOT}/pdfium/pdfium
                    ${TH_ROOT}/pdfium/third_party
                    ${TH_ROOT}/ZLib
                    ${TH_ROOT}/FreeImage/Source/LibJPEG
                    ${TH_ROOT}/FreeImage/Source/LibOpenJPEG
                    ${TH_ROOT}/freetype/freetype-${FREETYPE_VER}
                    ${TH_ROOT}/freetype/freetype-${FREETYPE_VER}/include)

IF(WIN32)
add_definitions(-DNOMINMAX)
ENDIF()

# PDFium definitions
include(definitions.txt)

IF(ODA_SHARED)
  #add_definitions(-DFPDFSDK_EXPORTS)
ENDIF()


#th_component(${TH_PDFIUM_LIB} ${TH_FT_LIB} ${TH_ZLIB_LIB} ${TD_ALLOC_LIB})

th_library(${TH_PDFIUM_LIB} ${TD_ALLOC_LIB})

th_project_group(${TH_PDFIUM_LIB} "PDFium")

set_property(TARGET ${TH_PDFIUM_LIB} APPEND_STRING PROPERTY COMPILE_FLAGS " -std=c++11")
