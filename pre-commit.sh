set -euxo pipefail

(cd server && yarn && yarn eslint)
(cd client && yarn && yarn eslint)