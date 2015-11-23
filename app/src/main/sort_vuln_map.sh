jq '[(.[] | (. * {time: .cvedate  | strptime("%m/%d/%Y") | strftime("%s") | tonumber}))] | sort_by(.time) | del(.[].time)' assets/vuln_map.json
