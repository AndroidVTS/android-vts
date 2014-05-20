import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.BufferedOutputStream;
import java.io.ByteArrayOutputStream;
import java.io.ByteArrayInputStream;
import java.io.FileWriter;
import java.io.IOException;
import java.io.InputStream;
import java.util.Enumeration;
import java.util.Random;
import java.util.zip.ZipEntry;
import java.util.zip.ZipException;
import java.util.zip.ZipFile;
import java.util.zip.ZipInputStream;
import java.util.zip.ZipOutputStream;

public class ZipBugChecker {
    static class VulnDeviceException extends Exception{
        private static final long serialVersionUID = 0x1337L;
        public VulnDeviceException(String message){
          super(message);
        }
    }

    public static void main(String args[]){
      try {
        testDuplicateEntries();
      }catch(VulnDeviceException e){
        System.out.println("Your device is vulnerable to bug 8219321");
        System.exit(-1);
      }catch(Exception e){
        System.out.println("There was an error running the test for bug 8219321 " + e.getMessage());
        System.exit(-1);
      }

       System.out.println("It seems your device is not vulnerable to bug 8219321");
       System.exit(0);
    }
    /**
     * Make sure we don't fail silently for duplicate entries.
     * b/8219321
     */
    public static void testDuplicateEntries() throws Exception {
        String entryName = "test_file_name1";
        String tmpName = "test_file_name2";

        // create the template data
        ByteArrayOutputStream bytesOut = new ByteArrayOutputStream();
        ZipOutputStream out = new ZipOutputStream(bytesOut);
        ZipEntry ze1 = new ZipEntry(tmpName);
        out.putNextEntry(ze1);
        out.closeEntry();
        ZipEntry ze2 = new ZipEntry(entryName);
        out.putNextEntry(ze2);
        out.closeEntry();
        out.close();

        // replace the bytes we don't like
        byte[] buf = bytesOut.toByteArray();
        replaceBytes(tmpName.getBytes(), entryName.getBytes(), buf);

        // write the result to a file
        File badZip = new File("/data/local/tmp/test.zip");
        badZip.deleteOnExit();
        FileOutputStream outstream = new FileOutputStream(badZip);
        outstream.write(buf);
        outstream.close();

        // see if we can still handle it
        try {
            ZipFile bad = new ZipFile(badZip);
            throw new VulnDeviceException("Vulnerable to zip bug 8219321");
        } catch (ZipException expected) {
            System.out.println(expected.getMessage());
        }
    }

    private static void replaceBytes(byte[] original, byte[] replacement, byte[] buffer) throws Exception {
        // Gotcha here: original and replacement must be the same length
        if(original.length != replacement.length)
           throw new Exception("Original Length is not equal to replacement length");

        boolean found;
        for(int i=0; i < buffer.length - original.length; i++) {
            found = false;
            if (buffer[i] == original[0]) {
                found = true;
                for (int j=0; j < original.length; j++) {
                    if (buffer[i+j] != original[j]) {
                        found = false;
                        break;
                    }
                }
            }
            if (found) {
                for (int j=0; j < original.length; j++) {
                    buffer[i+j] = replacement[j];
                }
            }
        }
    }
}
