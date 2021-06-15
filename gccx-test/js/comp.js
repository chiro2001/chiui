// import gccx from "gccx";
const gccx = require("gccx").default;
const fs = require("fs");
const path = require("path");

// console.log('gccx', gccx);

function delFile(path, reservePath) {
    if (fs.existsSync(path)) {
        if (fs.statSync(path).isDirectory()) {
            let files = fs.readdirSync(path);
            files.forEach((file, index) => {
                let currentPath = path + "/" + file;
                if (fs.statSync(currentPath).isDirectory()) {
                    delFile(currentPath, reservePath);
                } else {
                    fs.unlinkSync(currentPath);
                }
            });
            if (path !== reservePath) {
                fs.rmdirSync(path);
            }
        } else {
            fs.unlinkSync(path);
        }
    }
}

const readData = (filename) => {
    return new Promise((resolve, reject) => {
        fs.readFile(filename, 'utf8', (err, data) => {
            if (err) {
                console.error(err);
                reject(err);
                return;
            }
            resolve(data);
        });
    })
};

const files = process.argv.slice(2);
if (files.length === 0) {
    console.error('empty file list');
    process.exit(1);
}
// const patten = /\/\*[^*]*\*\//gims;
// const patten = /[a-z]\s/gims;
// const patten = /GCCX\s/gims;
const patten = new RegExp("GCCX", "g");
(async () => {
    for (const file of files) {
        const name = path.basename(file), ext = path.extname(file);
        const new_name = `${name.slice(0, name.lastIndexOf('.'))}_dist${ext}`;
        await delFile(new_name);
        console.log('file', file, '==>', new_name);
        let fileData = null;
        try {
            fileData = await readData(file);
            if (!fileData) {
                console.error('read file error:', e);
                process.exit(1);
                return;
            }
        } catch (e) {
            console.error('read file error:', e);
            process.exit(1);
            return;
        }
        console.log('read file done');
        let complied = '';
        try {
            complied = gccx.parse(fileData);
        } catch (e) {
            console.error(e);
            continue;
        }
        console.log('parse code done');
        // console.log(complied);
        // const matches = complied.match(new RegExp("GCCX[ \n\t]*/\\*([^\\\\*]|(\\*)*[^\\\\*/])*(\\*)*\\*/", "g"));
        const matches = complied.match(new RegExp("GCCX[ \n\t]*/\\*([^\\\\*]|(\\*)*[^*/])*(\\*)*\\*/", "g"));
        console.log('matches done');
        if (matches)
            for (const match of matches) {
                console.log('match', match);
                const replaced = match.replace(new RegExp("GCCX[ \n\t]*/\\*", "gims"), "")
                    .replace(new RegExp("\\*/", "gims"), "")
                    .replace(new RegExp("asmdom::", "gims"), "chidom::");
                complied = complied.replace(match, replaced);
            }
        else console.warn('error matches');
        // console.log(complied);
        fs.writeFile(new_name, complied, err => {
            err && console.error(err);
        })
    }
})();

