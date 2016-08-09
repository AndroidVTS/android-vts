package android.framework.util.jar;

import android.util.Log;

import java.io.File;
import java.io.InputStream;
import java.security.cert.Certificate;
import java.security.cert.X509Certificate;
import java.util.ArrayList;
import java.util.Enumeration;
import java.util.zip.ZipFile;

public class JarFileHelper {
    private JarFileHelper() {
    }

        public static Certificate[] getSignedJarCerts(String jarName, boolean chainCheck) throws Exception {
            File file = new File(jarName);
            Certificate[] foundCerts = null;

            JarFile jarFile = new JarFile(file, true, ZipFile.OPEN_READ, chainCheck);
            try {

                Enumeration<JarEntry> e = jarFile.entries();
                while (e.hasMoreElements()) {
                    JarEntry entry = e.nextElement();
                    InputStream is = jarFile.getInputStream(entry);
                    // Skip bytes because we have to read the entire file for it to read signatures.
                    is.skip(entry.getSize());
                    is.close();
                    Certificate[] certs = entry.getCertificates();
                    if (certs != null && certs.length > 0) {
                        foundCerts = certs;
                        break;
                    }
                }
            } finally {
                jarFile.close();
            }

            return foundCerts;
        }


    public static boolean isExploitingBug13678484(String apkName) throws Exception {
        ArrayList<String> validatedCertChain = new ArrayList<>();

        Certificate[] certs = JarFileHelper.getSignedJarCerts(apkName, true);
        for(Certificate c: certs)
            validatedCertChain.add(((X509Certificate)c).getSubjectDN().toString());

        ArrayList<String> unvalidatedCertChain = new ArrayList<>();

        Certificate[] certsfalse = JarFileHelper.getSignedJarCerts(apkName, false);
        for(Certificate c: certsfalse)
            unvalidatedCertChain.add(((X509Certificate)c).getSubjectDN().toString());

        return !validatedCertChain.equals(unvalidatedCertChain);

    }

}
