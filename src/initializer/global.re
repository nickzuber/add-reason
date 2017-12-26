
open GenericBindings;

module Emojis {
  let use = ref(true);

  let config = () =>  use^ ? getEmoji("truck") : "";
  let validate = () =>  use^ ? getEmoji("open_file_folder") : "";
  let linking = () =>  use^ ? getEmoji("link") : "";
  let postinstall = () =>  use^ ? getEmoji("nut_and_bolt") : "";
  let failure = () =>  use^ ? getEmoji("no_entry_sign") : "";
  let success = () =>  use^ ? getEmoji("sparkles") : "";
};
