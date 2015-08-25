package com.android.org.conscrypt;

import java.io.Serializable;

/**
 * Created by fuzion24 on 8/25/15.
 */
public class ZpenSSLX509Certificate implements Serializable {
    private static final long serialVersionUID = 0L;
    public final long mContext;
    public ZpenSSLX509Certificate(long ctx) {
        mContext = ctx;
    }
}
