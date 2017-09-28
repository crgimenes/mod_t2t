import txt2tags


def t2t(f):
    infiles_config = txt2tags.get_infiles_config([f])[0]
    infiles_config[0]['infile'] = infiles_config[0]['sourcefile']
    infiles_config[0]['toc_tagger'] = 0
    source_head, source_conf, source_body = infiles_config[1]
    myconf = txt2tags.ConfigMaster().sanity(infiles_config[0])
    target_head = txt2tags.doHeader(source_head, myconf)
    first_body_line = (len(source_head) or 1) + len(source_conf) + 1
    target_body, marked_toc = txt2tags.convert(source_body, myconf,
                                               firstlinenr=first_body_line)
    tagged_toc = txt2tags.toc_tagger(marked_toc, myconf)
    target_toc = txt2tags.toc_formatter(tagged_toc, myconf)
    target_body = txt2tags.toc_inside_body(target_body, target_toc, myconf)
    target_foot = txt2tags.doFooter(myconf)
    outlist = target_head + target_toc + target_body + target_foot
    return '\n'.join(outlist)
