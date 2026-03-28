const input = await Bun.file('./input.txt').text();

let char = [];
let charStart = Infinity;
let charEnd = 0;

for (const line of input.split('\n')) {
  if (line.startsWith('0x')) {
    const byte = parseInt(line, 16);
    const row = byte.toString(2).padStart(64, '0');
    char.push(row);

    const firstOneIndex = row.indexOf('1');
    if (firstOneIndex !== -1 && firstOneIndex < charStart) {
      charStart = firstOneIndex;
    }

    const lastOneIndex = row.lastIndexOf('1')+1;
    if (lastOneIndex > charEnd) {
      charEnd = lastOneIndex;
    }
  } else {
    if (char.length > 0) {
      console.log('CHAR(');
      for (const row of char) {
        console.log('0b' + row.slice(charStart, charEnd) + ',');
      }
      console.log('),');
    }

    char = [];
    charStart = Infinity;
    charEnd = 0;

    console.log(line);
  }
}
