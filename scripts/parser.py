#!/usr/bin/env python3

"""A parser for The Wall Street Journal documents."""

import re
import string
import sys

#-----------------------------------------------------------------------------------------------------------------------
DOCNO_TAG = '<DOCNO>'
DOC_END_TAG = '</DOC>'
TAGS_TO_PRINT = [ '<HL>', '<TEXT>', '<LP>' ]
PRINTABLE_CHARACTERS = set(string.printable)
SPLIT_TAGS_RE = re.compile(r'(<[^>]+>)')

# We remove from tokens such characters that usually serve as connectors in a sentence (comma, semicolon, etc.)
# We do not remove such characters that contain information about the token itself (dollar sign, apostrophe, etc.)
# Period and colon are special: we preserve them only before digits (7:30pm, .44-caliber)
SKIP_PUNCTUATION_RE = re.compile(r"[^\sa-z0-9#$%'&+/=.:<>]+")
SKIP_SPECIAL_RE = re.compile(r'[.:]+([^0-9]|$)')

#-----------------------------------------------------------------------------------------------------------------------
def skip_non_printable_chars(data):
    return ''.join(filter(lambda c: c in PRINTABLE_CHARACTERS, data))

def unescape_xml(data):
    data = data.replace('&amp;', '&')
    data = data.replace('&lt;', '<')
    data = data.replace('&gt;', '>')
    data = data.replace('&apos;', "'")
    data = data.replace('&quot;', '"')
    return data

def is_good_token(token):
    return any(c.isalnum() for c in token)

def normalize_text(text):
    text = unescape_xml(text)
    text = text.lower()
    text = text.replace('-', ' ')
    text = SKIP_PUNCTUATION_RE.sub('', text)
    text = SKIP_SPECIAL_RE.sub(r'\1', text)
    return text

def print_token(text, normalize):
    if not text:
        return
    if normalize:
        text = normalize_text(text)
    for token in text.split():
        if is_good_token(token):
            print(token)

def print_doc(doc_tags, docs_printed):
    if DOCNO_TAG not in doc_tags:
        return False
    if docs_printed > 0:
        print()
    print_token(doc_tags[DOCNO_TAG], normalize=False)
    for tag in TAGS_TO_PRINT:
        print_token(doc_tags.get(tag, ''), normalize=True)
    return True

#-----------------------------------------------------------------------------------------------------------------------
def main():
    tags_stack = []
    doc_tags = {}
    docs_printed = 0

    # Read the input
    for line in sys.stdin:
        line = line.strip()
        line = skip_non_printable_chars(line)
        tags_and_vals = list(filter(is_good_token, SPLIT_TAGS_RE.split(line)))

        # Iterate over tags and their values
        for tag_or_val in tags_and_vals:
            if tag_or_val.startswith('</') and tags_stack:
                # Closing tag
                tag = tag_or_val
                tags_stack.pop()
                if tag == DOC_END_TAG:
                    if print_doc(doc_tags, docs_printed):
                        docs_printed += 1
                    doc_tags = {}
            elif tag_or_val.startswith('<'):
                # Opening tag
                tag = tag_or_val
                tags_stack.append(tag)
            elif tags_stack:
                # Tag value
                val = tag_or_val
                tag = tags_stack[-1]
                if tag in TAGS_TO_PRINT or tag == DOCNO_TAG:
                    doc_tags[tag] = doc_tags.get(tag, '') + ' ' + val

#-----------------------------------------------------------------------------------------------------------------------
if __name__ == '__main__':
    main()
