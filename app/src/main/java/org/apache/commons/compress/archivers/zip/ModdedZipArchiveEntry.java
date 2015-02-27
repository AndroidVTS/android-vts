
package org.apache.commons.compress.archivers.zip;

public class ModdedZipArchiveEntry extends ZipArchiveEntry {

    private byte[] rawExtra = null;

    public ModdedZipArchiveEntry(String name){
        super(name);
        rawExtra = new byte[]{};
    }

    public byte[] getRawCentralDirectoryExtra(){
        return rawExtra;
    }

    public void setRawCentralDirectoryExtra(byte[] rawExtra){
        this.rawExtra = rawExtra;
    }
}