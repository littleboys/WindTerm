/**********************************************************************
  euc_jp.c -  Onigmo (Oniguruma-mod) (regular expression library)
**********************************************************************/
/*-
 * Copyright (c) 2002-2008  K.Kosako  <sndgk393 AT ybb DOT ne DOT jp>
 * Copyright (c) 2011       K.Takata  <kentkt AT csc DOT jp>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include "regint.h"

#define eucjp_islead(c)    ((UChar )((c) - 0xa1) > 0xfe - 0xa1)

static const int EncLen_EUCJP[] = {
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 3,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1
};

static const OnigPairCaseFoldCodes CaseFoldMap[] = {
  /* Fullwidth Alphabet */
  { 0xa3c1, 0xa3e1 },
  { 0xa3c2, 0xa3e2 },
  { 0xa3c3, 0xa3e3 },
  { 0xa3c4, 0xa3e4 },
  { 0xa3c5, 0xa3e5 },
  { 0xa3c6, 0xa3e6 },
  { 0xa3c7, 0xa3e7 },
  { 0xa3c8, 0xa3e8 },
  { 0xa3c9, 0xa3e9 },
  { 0xa3ca, 0xa3ea },
  { 0xa3cb, 0xa3eb },
  { 0xa3cc, 0xa3ec },
  { 0xa3cd, 0xa3ed },
  { 0xa3ce, 0xa3ee },
  { 0xa3cf, 0xa3ef },
  { 0xa3d0, 0xa3f0 },
  { 0xa3d1, 0xa3f1 },
  { 0xa3d2, 0xa3f2 },
  { 0xa3d3, 0xa3f3 },
  { 0xa3d4, 0xa3f4 },
  { 0xa3d5, 0xa3f5 },
  { 0xa3d6, 0xa3f6 },
  { 0xa3d7, 0xa3f7 },
  { 0xa3d8, 0xa3f8 },
  { 0xa3d9, 0xa3f9 },
  { 0xa3da, 0xa3fa },

  /* Greek */
  { 0xa6a1, 0xa6c1 },
  { 0xa6a2, 0xa6c2 },
  { 0xa6a3, 0xa6c3 },
  { 0xa6a4, 0xa6c4 },
  { 0xa6a5, 0xa6c5 },
  { 0xa6a6, 0xa6c6 },
  { 0xa6a7, 0xa6c7 },
  { 0xa6a8, 0xa6c8 },
  { 0xa6a9, 0xa6c9 },
  { 0xa6aa, 0xa6ca },
  { 0xa6ab, 0xa6cb },
  { 0xa6ac, 0xa6cc },
  { 0xa6ad, 0xa6cd },
  { 0xa6ae, 0xa6ce },
  { 0xa6af, 0xa6cf },
  { 0xa6b0, 0xa6d0 },
  { 0xa6b1, 0xa6d1 },
  { 0xa6b2, 0xa6d2 },
  { 0xa6b3, 0xa6d3 },
  { 0xa6b4, 0xa6d4 },
  { 0xa6b5, 0xa6d5 },
  { 0xa6b6, 0xa6d6 },
  { 0xa6b7, 0xa6d7 },
  { 0xa6b8, 0xa6d8 },

  /* Cyrillic */
  { 0xa7a1, 0xa7d1 },
  { 0xa7a2, 0xa7d2 },
  { 0xa7a3, 0xa7d3 },
  { 0xa7a4, 0xa7d4 },
  { 0xa7a5, 0xa7d5 },
  { 0xa7a6, 0xa7d6 },
  { 0xa7a7, 0xa7d7 },
  { 0xa7a8, 0xa7d8 },
  { 0xa7a9, 0xa7d9 },
  { 0xa7aa, 0xa7da },
  { 0xa7ab, 0xa7db },
  { 0xa7ac, 0xa7dc },
  { 0xa7ad, 0xa7dd },
  { 0xa7ae, 0xa7de },
  { 0xa7af, 0xa7df },
  { 0xa7b0, 0xa7e0 },
  { 0xa7b1, 0xa7e1 },
  { 0xa7b2, 0xa7e2 },
  { 0xa7b3, 0xa7e3 },
  { 0xa7b4, 0xa7e4 },
  { 0xa7b5, 0xa7e5 },
  { 0xa7b6, 0xa7e6 },
  { 0xa7b7, 0xa7e7 },
  { 0xa7b8, 0xa7e8 },
  { 0xa7b9, 0xa7e9 },
  { 0xa7ba, 0xa7ea },
  { 0xa7bb, 0xa7eb },
  { 0xa7bc, 0xa7ec },
  { 0xa7bd, 0xa7ed },
  { 0xa7be, 0xa7ee },
  { 0xa7bf, 0xa7ef },
  { 0xa7c0, 0xa7f0 },
  { 0xa7c1, 0xa7f1 },
};

static int
mbc_enc_len(const UChar* p)
{
  return EncLen_EUCJP[*p];
}

static int
mbc_enc_len_se(OnigIterator* it, OnigPosition p)
{
  return EncLen_EUCJP[ONIG_CHARAT(p)];
}

static OnigCodePoint
mbc_to_code(const UChar* p, const UChar* end)
{
  int c, i, len;
  OnigCodePoint n;

  len = mbc_enc_len(p);
  n = (OnigCodePoint )*p++;
  if (len == 1) return n;

  for (i = 1; i < len; i++) {
    if (p >= end) break;
    c = *p++;
    n <<= 8;  n += c;
  }
  return n;
}

static OnigCodePoint
mbc_to_code_se(OnigIterator* it, OnigPosition p, OnigPosition end)
{
  int c, i, len;
  OnigCodePoint n;

  len = mbc_enc_len_se(it, p);
  n = (OnigCodePoint )ONIG_CHARAT(p++);
  if (len == 1) return n;

  for (i = 1; i < len; i++) {
    if (p >= end) break;
    c = ONIG_CHARAT(p++);
    n <<= 8;  n += c;
  }
  return n;
}

static int
code_to_mbclen(OnigCodePoint code)
{
  if (ONIGENC_IS_CODE_ASCII(code)) return 1;
  else if ((code & 0xff808080) == 0x00808080) return 3;
  else if ((code & 0xffff8080) == 0x00008080) return 2;
  else
    return ONIGERR_INVALID_CODE_POINT_VALUE;
}

#if 0
static int
code_to_mbc_first(OnigCodePoint code)
{
  int first;

  if ((code & 0xff0000) != 0) {
    first = (code >> 16) & 0xff;
  }
  else if ((code & 0xff00) != 0) {
    first = (code >> 8) & 0xff;
  }
  else {
    return (int )code;
  }
  return first;
}
#endif

static int
code_to_mbc(OnigCodePoint code, UChar *buf)
{
  UChar *p = buf;

  if ((code & 0xff0000) != 0) *p++ = (UChar )(((code >> 16) & 0xff));
  if ((code &   0xff00) != 0) *p++ = (UChar )(((code >>  8) & 0xff));
  *p++ = (UChar )(code & 0xff);

#if 1
  if (mbc_enc_len(buf) != (p - buf))
    return ONIGERR_INVALID_CODE_POINT_VALUE;
#endif
  return (int )(p - buf);
}

static int
apply_all_case_fold(OnigCaseFoldType flag,
		    OnigApplyAllCaseFoldFunc f, void* arg)
{
  return onigenc_apply_all_case_fold_with_map(
            sizeof(CaseFoldMap)/sizeof(OnigPairCaseFoldCodes), CaseFoldMap, 0,
            flag, f, arg);
}

static OnigCodePoint
get_lower_case(OnigCodePoint code)
{
  if (ONIGENC_IS_IN_RANGE(code, 0xa3c1, 0xa3da)) {
    /* Fullwidth Alphabet */
    return (OnigCodePoint )(code + 0x0020);
  }
  else if (ONIGENC_IS_IN_RANGE(code, 0xa6a1, 0xa6b8)) {
    /* Greek */
    return (OnigCodePoint )(code + 0x0020);
  }
  else if (ONIGENC_IS_IN_RANGE(code, 0xa7a1, 0xa7c1)) {
    /* Cyrillic */
    return (OnigCodePoint )(code + 0x0030);
  }
  return code;
}

static OnigCodePoint
get_upper_case(OnigCodePoint code)
{
  if (ONIGENC_IS_IN_RANGE(code, 0xa3e1, 0xa3fa)) {
    /* Fullwidth Alphabet */
    return (OnigCodePoint )(code - 0x0020);
  }
  else if (ONIGENC_IS_IN_RANGE(code, 0xa6c1, 0xa6d8)) {
    /* Greek */
    return (OnigCodePoint )(code - 0x0020);
  }
  else if (ONIGENC_IS_IN_RANGE(code, 0xa7d1, 0xa7f1)) {
    /* Cyrillic */
    return (OnigCodePoint )(code - 0x0030);
  }
  return code;
}

static int
get_case_fold_codes_by_str(OnigCaseFoldType flag,
			   const OnigUChar* p, const OnigUChar* end,
			   OnigCaseFoldCodeItem items[])
{
  int len;
  OnigCodePoint code, code_lo, code_up;

  code = mbc_to_code(p, end);
  if (ONIGENC_IS_ASCII_CODE(code))
    return onigenc_ascii_get_case_fold_codes_by_str(flag, p, end, items);

  len = mbc_enc_len(p);
  code_lo = get_lower_case(code);
  code_up = get_upper_case(code);

  if (code != code_lo) {
    items[0].byte_len = len;
    items[0].code_len = 1;
    items[0].code[0] = code_lo;
    return 1;
  }
  else if (code != code_up) {
    items[0].byte_len = len;
    items[0].code_len = 1;
    items[0].code[0] = code_up;
    return 1;
  }

  return 0;
}

static int
mbc_case_fold(OnigCaseFoldType flag ARG_UNUSED,
	      const UChar** pp, const UChar* end ARG_UNUSED, UChar* lower)
{
  const UChar* p = *pp;

  if (ONIGENC_IS_MBC_ASCII(p)) {
    *lower = ONIGENC_ASCII_CODE_TO_LOWER_CASE(*p);
    (*pp)++;
    return 1;
  }
  else {
    OnigCodePoint code;
    int len;

    code = get_lower_case(mbc_to_code(p, end));
    len = code_to_mbc(code, lower);
    (*pp) += len;
    return len; /* return byte length of converted char to lower */
  }
}

static int
mbc_case_fold_se(OnigIterator* it, OnigCaseFoldType flag ARG_UNUSED,
	      OnigPosition* pp, OnigPosition end ARG_UNUSED, UChar* lower)
{
  const UChar c = ONIG_CHARAT(*pp);

  if (ONIGENC_IS_MBC_ASCII_SE(c)) {
    *lower = ONIGENC_ASCII_CODE_TO_LOWER_CASE(c);
    (*pp)++;
    return 1;
  }
  else {
    OnigCodePoint code;
    int len;

    code = get_lower_case(mbc_to_code_se(it, *pp, end));
    len = code_to_mbc(code, lower);
    (*pp) += len;
    return len; /* return byte length of converted char to lower */
  }
}

static UChar*
left_adjust_char_head(const UChar* start, const UChar* s)
{
  /* In this encoding
     mb-trail bytes doesn't mix with single bytes.
  */
  const UChar *p;
  int len;

  if (s <= start) return (UChar* )s;
  p = s;

  while (!eucjp_islead(*p) && p > start) p--;
  len = mbc_enc_len(p);
  if (p + len > s) return (UChar* )p;
  p += len;
  return (UChar* )(p + ((s - p) & ~1));
}

static OnigPosition
left_adjust_char_head_se(OnigIterator* it, OnigPosition start, OnigPosition s)
{
  /* In this encoding
     mb-trail bytes doesn't mix with single bytes.
  */
  OnigPosition p;
  int len;

  if (s <= start) return s;
  p = s;

  while (!eucjp_islead(ONIG_CHARAT(p)) && p > start) p--;
  len = mbc_enc_len_se(it, p);
  if (p + len > s) return p;
  p += len;
  return (p + ((s - p) & ~1));
}

static int
is_allowed_reverse_match(const UChar* s, const UChar* end ARG_UNUSED)
{
  const UChar c = *s;
  if (c <= 0x7e || c == 0x8e || c == 0x8f)
    return TRUE;
  else
    return FALSE;
}


static int PropertyInited = 0;
static const OnigCodePoint** PropertyList;
static int PropertyListNum;
static int PropertyListSize;
static hash_table_type* PropertyNameTable;

static const OnigCodePoint CR_Hiragana[] = {
  1,
#ifdef ENC_EUC_JIS_2004
  0xa4a1, 0xa4fb
#else
  0xa4a1, 0xa4f3
#endif
}; /* CR_Hiragana */

#ifdef ENC_EUC_JIS_2004
static const OnigCodePoint CR_Katakana[] = {
  5,
  0x8ea6, 0x8eaf,	/* JIS X 0201 Katakana */
  0x8eb1, 0x8edd,	/* JIS X 0201 Katakana */
  0xa5a1, 0xa5fe,
  0xa6ee, 0xa6fe,
  0xa7f2, 0xa7f5,
}; /* CR_Katakana */
#else
static const OnigCodePoint CR_Katakana[] = {
  3,
  0x8ea6, 0x8eaf,	/* JIS X 0201 Katakana */
  0x8eb1, 0x8edd,	/* JIS X 0201 Katakana */
  0xa5a1, 0xa5f6,
}; /* CR_Katakana */
#endif

#ifdef ENC_EUC_JIS_2004
static const OnigCodePoint CR_Han[] = {
  /* EUC-JIS-2004 (JIS X 0213:2004) */
  7,
  /* plane 1 */
  0xa1b8, 0xa1b8,
  0xaea1, 0xfefe,	/* Kanji level 1, 2 and 3 */
  /* plane 2 */
  0x8fa1a1, 0x8fa1fe,	/* row 1 */
  0x8fa3a1, 0x8fa5fe,	/* row 3 .. 5 */
  0x8fa8a1, 0x8fa8fe,	/* row 8 */
  0x8faca1, 0x8faffe,	/* row 12 .. 15 */
  0x8feea1, 0x8ffef6,	/* row 78 .. 94 */
}; /* CR_Han */
#else
static const OnigCodePoint CR_Han[] = {
  /* EUC-JP (JIS X 0208 based) */
  4,
  0xa1b8, 0xa1b8,
  0xb0a1, 0xcfd3,	/* Kanji level 1 */
  0xd0a1, 0xf4a6,	/* Kanji level 2 */
  0x8fb0a1, 0x8fedf3	/* JIS X 0212 Supplemental Kanji (row 16 .. 77) */
}; /* CR_Han */
#endif

static const OnigCodePoint CR_Latin[] = {
  4,
  0x0041, 0x005a,
  0x0061, 0x007a,
  0xa3c1, 0xa3da,
  0xa3e1, 0xa3fa,
  /* TODO: add raw 8 .. 11 to support EUC-JIS-2004 */
  /* TODO: add JIS X 0212 row 9 .. 11 */
}; /* CR_Latin */

static const OnigCodePoint CR_Greek[] = {
  2,
  0xa6a1, 0xa6b8,
#ifdef ENC_EUC_JIS_2004
  0xa6c1, 0xa6d9,
#else
  0xa6c1, 0xa6d8,
  /* TODO: add JIS X 0212 row 6 */
#endif
}; /* CR_Greek */

static const OnigCodePoint CR_Cyrillic[] = {
  2,
  0xa7a1, 0xa7c1,
  0xa7d1, 0xa7f1,
  /* TODO: add JIS X 0212 row 7 */
}; /* CR_Cyrillic */

static int
init_property_list(void)
{
  int r;

  PROPERTY_LIST_ADD_PROP("hiragana", CR_Hiragana);
  PROPERTY_LIST_ADD_PROP("katakana", CR_Katakana);
  PROPERTY_LIST_ADD_PROP("han", CR_Han);
  PROPERTY_LIST_ADD_PROP("latin", CR_Latin);
  PROPERTY_LIST_ADD_PROP("greek", CR_Greek);
  PROPERTY_LIST_ADD_PROP("cyrillic", CR_Cyrillic);
  PropertyInited = 1;

 end:
  return r;
}

static int
property_name_to_ctype(OnigEncoding enc, UChar* p, UChar* end)
{
  hash_data_type ctype;
  UChar *s, *e;

  PROPERTY_LIST_INIT_CHECK;

  s = e = xalloca(end - p + 1);
  for (; p < end; p++) {
    *e++ = ONIGENC_ASCII_CODE_TO_LOWER_CASE(*p);
  }

  if (onig_st_lookup_strend(PropertyNameTable, s, e, &ctype) == 0) {
    return onigenc_minimum_property_name_to_ctype(enc, s, e);
  }

  return (int )ctype;
}

static int
is_code_ctype(OnigCodePoint code, unsigned int ctype)
{
  if (ctype <= ONIGENC_MAX_STD_CTYPE) {
    if (code < 128)
      return ONIGENC_IS_ASCII_CODE_CTYPE(code, ctype);
    else {
      if (CTYPE_IS_WORD_GRAPH_PRINT(ctype)) {
	return (code_to_mbclen(code) > 1 ? TRUE : FALSE);
      }
    }
  }
  else {
    PROPERTY_LIST_INIT_CHECK;

    ctype -= (ONIGENC_MAX_STD_CTYPE + 1);
    if (ctype >= (unsigned int )PropertyListNum)
      return ONIGERR_TYPE_BUG;

    return onig_is_in_code_range((UChar* )PropertyList[ctype], code);
  }

  return FALSE;
}

static int
get_ctype_code_range(OnigCtype ctype, OnigCodePoint* sb_out,
		     const OnigCodePoint* ranges[])
{
  if (ctype <= ONIGENC_MAX_STD_CTYPE) {
    return ONIG_NO_SUPPORT_CONFIG;
  }
  else {
    *sb_out = 0x80;

    PROPERTY_LIST_INIT_CHECK;

    ctype -= (ONIGENC_MAX_STD_CTYPE + 1);
    if (ctype >= (OnigCtype )PropertyListNum)
      return ONIGERR_TYPE_BUG;

    *ranges = PropertyList[ctype];
    return 0;
  }
}


OnigEncodingType OnigEncodingEUC_JP = {
  mbc_enc_len,
  mbc_enc_len_se,
  "EUC-JP",   /* name */
  3,          /* max enc length */
  1,          /* min enc length */
  onigenc_is_mbc_newline_0x0a,
  onigenc_is_mbc_newline_0x0a_se,
  mbc_to_code,
  mbc_to_code_se,
  code_to_mbclen,
  code_to_mbc,
  mbc_case_fold,
  mbc_case_fold_se,
  apply_all_case_fold,
  get_case_fold_codes_by_str,
  property_name_to_ctype,
  is_code_ctype,
  get_ctype_code_range,
  left_adjust_char_head,
  left_adjust_char_head_se,
  is_allowed_reverse_match,
  ONIGENC_FLAG_NONE,
};
