const fs = require('fs');
const path = require('path');

const srcURL = (src_path) => 'C:\\Users\\NPShinigami\\source\\repos\\' + src_path;
const dstURL = (dst_path) => 'E:\\Programacao\\cpp\\' + dst_path;

// E:\Programacao\cpp\TestScript\FluffyScript\src\fluffy
let sourceOriginDest = [
	[
		srcURL('FluffyScript\\FluffyScript'),
		dstURL('FluffyScript\\src\\fluffy'),
		true,
	],
	[
		srcURL('FluffyScript\\FluffyScript\\parser_objects'),
		dstURL('FluffyScript\\src\\fluffy\\parser_objects'),
	],
	[
		srcURL('FluffyScript\\FluffyScript\\parser_objects\\objects'),
		dstURL('FluffyScript\\src\\fluffy\\parser_objects\\objects'),
	],
	[
		srcURL('FluffyScriptTest\\tests'),
		dstURL('FluffyScriptTest\\src\\tests')
	],
	[
		srcURL('FluffyScriptTest\\files'),
		dstURL('FluffyScriptTest\\src\\files'),
	],
];

function parseDirectory(inputDir, outputDir, needFilter)
{
	fs.readdir(inputDir, (err, entries) => {
		if (err)
		{
			throw Error(err);
		}
		if (needFilter) {
			entries.forEach((entry) => {
				if (entry.match(/fl_(\w*).(h|cpp)/))
				{
					const inputEntryPath = `${inputDir}\\${entry}`;
					const outputEntryPath = `${outputDir}\\${entry}`;
					
					fs.stat(inputEntryPath, (statErr, stat) => {
						if (stat.isDirectory()) {
							parseDirectory(inputEntryPath);
						} else {
							parseFile(inputDir, outputDir, entry);
						}
					});
				}
			});
		} else {
			entries.forEach((entry) => {
				const inputEntryPath = `${inputDir}\\${entry}`;
				const outputEntryPath = `${outputDir}\\${entry}`;
				
				fs.stat(inputEntryPath, (statErr, stat) => {
					if (statErr)
					{
						throw Error(statErr);
					}
					if (stat.isDirectory()) {
						parseDirectory(inputEntryPath, outputEntryPath);
					} else {
						parseFile(inputDir, outputDir, entry);
					}
				});
			});
		}
	});
}

function parseFile(inputDir, outputDir, entry)
{
	const inputFilePath = `${inputDir}\\${entry}`;
	const outputFilePath = `${outputDir}\\${entry}`;
	
	fs.stat(outputDir, async (err, statDir) => {
		if (err)
		{
			if (err?.code === 'ENOENT')
			{
				await fs.mkdir(outputDir, { recursive: true }, async (errMkdir) => {
					if (errMkdir) {
						throw Error('Falha ao criar diretorio: ' + outputDir);
					} else {
						await fs.copyFile(inputFilePath, outputFilePath, (err) => {
							if (err) {
								throw Error('Falha ao copiar: ' + inputFilePath);
							}
							console.log(`copied: ${entry}`);
						});
					}
				});
			}
		} else {
			await fs.copyFile(inputFilePath, outputFilePath, (err) => {			
				if (err) {
					throw Error('Falha ao copiar: ' + inputFilePath);
				}
				console.log(`copied: ${entry}`);
			});
		}
	});
}

sourceOriginDest.forEach((source) => {
	parseDirectory(source[0], source[1], source[2]);
});



















