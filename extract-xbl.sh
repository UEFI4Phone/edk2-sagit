#!/bin/bash

EXTRACTED_DIR="./workspace/xbl_extract/fv_extracted"
DEST_DIR="./XiaomiMI6Pkg/Binary"

cleanup() {

	rm -rf ./workspace/xbl_extract

}

extract_xbl() {

	mkdir -p ./workspace/xbl_extract
	cp ./xbl.elf ./workspace/xbl_extract/xbl.elf
	cd ./workspace/xbl_extract
	7z x xbl.elf -r -o./xbl
	7z x ./xbl/11 -r -o./uefi_fv
	uefi-firmware-parser -e -b --verbose -o ./fv_extracted ./uefi_fv/11~
	cd ../../

}

copy_binary_files() {
	
	cp $EXTRACTED_DIR/volume-8/file-90a49afd-422f-08ae-9611-e788d3804845/section1.pe $DEST_DIR/EnvDxe/EnvDxe.efi
	cp $EXTRACTED_DIR/volume-8/file-5e0eae60-eaed-4d75-b8bf-edbbaabc3f09/section0.pe $DEST_DIR/SecurityDxe/SecurityDxe.efi
	cp $EXTRACTED_DIR/volume-8/file-c2f9a4f5-f7b4-43e7-ba99-5ea804cc103a/section0.pe $DEST_DIR/ASN1X509Dxe/ASN1X509Dxe.efi
	cp $EXTRACTED_DIR/volume-8/file-32c71e68-83a8-46ed-aed1-094e71b12057/section0.pe $DEST_DIR/SecRSADxe/SecRSADxe.efi
	cp $EXTRACTED_DIR/volume-8/file-f541d663-4a48-40aa-aabf-ff158ccae34c/section1.pe $DEST_DIR/SmemDxe/SmemDxe.efi
	cp $EXTRACTED_DIR/volume-8/file-8e9bd160-b184-11df-94e2-0800200c9a66/section1.pe $DEST_DIR/DALSys/DALSys.efi
	cp $EXTRACTED_DIR/volume-8/file-af9763a2-033b-4109-8e17-56a98d380c92/section1.pe $DEST_DIR/HWIODxeDriver/HWIODxeDriver.efi
	cp $EXTRACTED_DIR/volume-8/file-10e193df-9966-44e7-b17c-59dd831e20fc/section1.pe $DEST_DIR/ChipInfo/ChipInfo.efi
	cp $EXTRACTED_DIR/volume-8/file-b105211b-bbbd-4add-a3b0-d1cf4a52154c/section1.pe $DEST_DIR/PlatformInfoDxeDriver/PlatformInfoDxeDriver.efi
	cp $EXTRACTED_DIR/volume-8/file-7942a7eb-b7d5-4e2e-b908-831e4de55b58/section1.pe $DEST_DIR/GlinkDxe/GlinkDxe.efi
	cp $EXTRACTED_DIR/volume-8/file-e43128a8-8692-42b6-8afa-676158578d18/section1.pe $DEST_DIR/ULogDxe/ULogDxe.efi
	cp $EXTRACTED_DIR/volume-8/file-cb29f4d1-7f37-4692-a416-93e82e219711/section1.pe $DEST_DIR/NpaDxe/NpaDxe.efi
	cp $EXTRACTED_DIR/volume-8/file-4db5dea6-5302-4d1a-8a82-677a683b0d29/section1.pe $DEST_DIR/ClockDxe/ClockDxe.efi
	cp $EXTRACTED_DIR/volume-8/file-f10f76db-42c1-533f-34a8-69be24653110/section1.pe $DEST_DIR/SdccDxe/SdccDxe.efi
	cp $EXTRACTED_DIR/volume-8/file-0d35cd8e-97ea-4f9a-96af-0f0d89f76567/section1.pe $DEST_DIR/UFSDxe/UFSDxe.efi
	cp $EXTRACTED_DIR/volume-8/file-b003d837-44cc-b38b-7811-deb5ebbd74d8/section1.pe $DEST_DIR/QdssDxe/QdssDxe.efi
	cp $EXTRACTED_DIR/volume-8/file-02b01ad5-7e59-43e8-a6d8-238180613a5a/section1.pe $DEST_DIR/EmuVariableRuntimeDxe/EmuVariableRuntimeDxe.efi
	cp $EXTRACTED_DIR/volume-8/file-8681cc5a-0df6-441e-b4b8-e915c538f067/section1.pe $DEST_DIR/DALTLMM/DALTLMM.efi
	cp $EXTRACTED_DIR/volume-8/file-04de8591-d2b3-4077-bbbe-b12070094eb6/section1.pe $DEST_DIR/I2C/I2C.efi
	cp $EXTRACTED_DIR/volume-8/file-2a7b4bef-80cd-49e1-b473-374ba4d673fc/section1.pe $DEST_DIR/SPMI/SPMI.efi
	cp $EXTRACTED_DIR/volume-8/file-7a32bd23-f735-4f57-aa1a-447d2fe3be0d/section1.pe $DEST_DIR/SPI/SPI.efi
	cp $EXTRACTED_DIR/volume-8/file-5776232e-082d-4b75-9a0e-fe1d13f7a5d9/section1.pe $DEST_DIR/PmicDxe/PmicDxe.efi
	cp $EXTRACTED_DIR/volume-8/file-5a5cb8ca-9382-4e0c-b383-77fb517cd9eb/section1.pe $DEST_DIR/AdcDxe/AdcDxe.efi
	cp $EXTRACTED_DIR/volume-8/file-4bce7f36-428e-4393-99e3-7e0844404dba/section1.pe $DEST_DIR/QcomChargerDxeLA/QcomChargerDxeLA.efi
	cp $EXTRACTED_DIR/volume-8/file-5bd181db-0487-4f1a-ae73-820e165611b3/section1.pe $DEST_DIR/ButtonsDxe/ButtonsDxe.efi
	cp $EXTRACTED_DIR/volume-8/file-3adf8dda-1850-44c5-8c63-bb991849bc6f/section0.pe $DEST_DIR/HashDxe/HashDxe.efi
	cp $EXTRACTED_DIR/volume-8/file-b0d3689e-11f8-43c6-8ece-023a29cec35b/section0.pe $DEST_DIR/RngDxe/RngDxe.efi


}

patch_binary_files() {

	bspatch $DEST_DIR/ClockDxe/ClockDxe.efi $DEST_DIR/ClockDxe/ClockDxe.patched.efi $DEST_DIR/ClockDxe/ClockDxe.diff
	rm $DEST_DIR/ClockDxe/ClockDxe.efi
	mv $DEST_DIR/ClockDxe/ClockDxe.patched.efi $DEST_DIR/ClockDxe/ClockDxe.efi

}

cleanup
extract_xbl
copy_binary_files
patch_binary_files
