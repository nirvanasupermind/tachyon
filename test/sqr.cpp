ErisVal sqr(ErisVal x) {
    assert(x.tag == ErisVal::NUM);
    return ErisVal::make_num(x.n * x.n);
}