void
apply_fribidi(char *str)
{
  FriBidiStrIndex len = strlen(str);
  FriBidiChar logical[BUFSIZ];
  FriBidiChar visual[BUFSIZ];
  FriBidiParType base = FRIBIDI_PAR_ON;
  FriBidiCharSet charset;
  fribidi_boolean result;

  fribidi_text[0] = 0;
  if (len>0)
  {
    charset = fribidi_parse_charset("UTF-8");
    len = fribidi_charset_to_unicode(charset, str, len, logical);
    result = fribidi_log2vis(logical, len, &base, visual, NULL, NULL, NULL);
    len = fribidi_unicode_to_charset(charset, visual, len, fribidi_text);
  }
}
